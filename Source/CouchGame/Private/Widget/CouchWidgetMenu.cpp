#include "Widget/CouchWidgetMenu.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCouchWidgetMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (Btn_Quit)
	{
		Btn_Quit->OnPressed.AddDynamic(this, &UCouchWidgetMenu::OnQuitPressed);	
	}
	if (Btn_Option)
	{
		Btn_Option->OnPressed.AddDynamic(this, &UCouchWidgetMenu::OnOptionPressed);	
	}
	if (Btn_Play)
	{
		Btn_Play->OnPressed.AddDynamic(this, &UCouchWidgetMenu::OnPlayPressed);	
	}
	return true;
}

void UCouchWidgetMenu::OnQuitPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UCouchWidgetMenu::OnOptionPressed()
{
}

void UCouchWidgetMenu::OnPlayPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), LobbyName);
}
