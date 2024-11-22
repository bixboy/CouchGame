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
		FTimerHandle RoundTimerHandle;
		PlayAnimationForward(OpenAnimation);
		GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, [this]() { EndAnimation(true); }, 2.f, false);	
	}
}

void UCouchWidgetPause::EndAnimation(bool Pause)
{
	UGameplayStatics::SetGamePaused(GetWorld(), Pause);
	if (!Pause)
		RemoveFromParent();
}

void UCouchWidgetPause::OnQuitPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UCouchWidgetPause::OnResumePressed()
{
	FTimerHandle RoundTimerHandle;
	PlayAnimationReverse(OpenAnimation);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, [this]() { EndAnimation(false); }, OpenAnimation->GetEndTime(), false);
}

void UCouchWidgetPause::OnMenuPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), MenuName);
}
