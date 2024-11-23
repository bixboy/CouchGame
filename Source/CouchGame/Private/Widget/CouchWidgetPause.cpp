#include "Widget/CouchWidgetPause.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCouchWidgetPause::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (Btn_Quit)
	{
		Btn_Quit->OnPressed.AddDynamic(this, &UCouchWidgetPause::OnQuitPressed);	
	}
	if (Btn_Resume)
	{
		Btn_Resume->OnPressed.AddDynamic(this, &UCouchWidgetPause::OnResumePressed);	
	}
	if (Btn_Menu)
	{
		Btn_Menu->OnPressed.AddDynamic(this, &UCouchWidgetPause::OnMenuPressed);	
	}
	return true;
}

void UCouchWidgetPause::NativeConstruct()
{
	Super::NativeConstruct();
	if (OpenAnimation)
	{
		PlayAnimationForward(OpenAnimation);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void UCouchWidgetPause::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bIsResumePending && !IsAnimationPlaying(OpenAnimation))
	{
		// Reprend le jeu
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		RemoveFromParent();
		bIsResumePending = false;
	}
}

void UCouchWidgetPause::OnQuitPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UCouchWidgetPause::OnResumePressed()
{
	PlayAnimationReverse(OpenAnimation);
	bIsResumePending = true;
}

void UCouchWidgetPause::OnMenuPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), MenuName);
}

void UCouchWidgetPause::Resume()
{
	OnResumePressed();
}
