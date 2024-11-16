#include "Widget/CouchWidgetMenu.h"
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
	if (Btn_Option)
	{
		Btn_Option->OnPressed.AddDynamic(this, &UCouchWidgetPause::OnOptionPressed);	
	}
	if (Btn_Menu)
	{
		Btn_Play->OnPressed.AddDynamic(this, &UCouchWidgetPause::OnPlayPressed);	
	}
	return true;
}

void UCouchWidgetPause::OnQuitPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UCouchWidgetPause::OnOptionPressed()
{
}

void UCouchWidgetPause::OnPlayPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), LobbyName);
}
