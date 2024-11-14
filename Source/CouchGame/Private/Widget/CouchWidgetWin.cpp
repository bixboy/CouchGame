
#include "Widget/CouchWidgetWin.h"
#include "Arena/CouchGameManagerSubSystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UCouchWidgetWin::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UCouchWidgetWin::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	if (Btn_NewRound)
	{
		Btn_NewRound->OnPressed.AddDynamic(this, &UCouchWidgetWin::OnNewRoundPressed);	
	}
	return true;
}

void UCouchWidgetWin::ChangeWinnerText(FText Winner)
{
	if (Txt_Winner)
	{
		Txt_Winner->SetText(Winner);
	}
}

void UCouchWidgetWin::OnNewRoundPressed()
{
	UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	if (GameManager)
	{
		GameManager->StartNewRound();
	}
}
