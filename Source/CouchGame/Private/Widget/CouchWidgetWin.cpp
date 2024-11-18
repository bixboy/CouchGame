
#include "Widget/CouchWidgetWin.h"
#include "Animation/WidgetAnimation.h"
#include "Arena/CouchGameManagerSubSystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UCouchWidgetWin::NativeConstruct()
{
	Super::NativeConstruct();
	if (OpenAnimation)
	{
		FTimerHandle RoundTimerHandle;
		PlayAnimationForward(OpenAnimation);
		GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, [this]() { EndAnimation(); }, OpenAnimation->GetEndTime(), false);	
	}
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

#pragma region Button & Text

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

#pragma endregion


void UCouchWidgetWin::EndAnimation()
{
	PlayAnimationForward(TextAppearAnimation);
	StartConfetti();
}

void UCouchWidgetWin::StartConfetti()
{
	NSWidget_0->ActivateSystem(true);
	NSWidget_1->ActivateSystem(true);
	NSWidget_2->ActivateSystem(true);
}
