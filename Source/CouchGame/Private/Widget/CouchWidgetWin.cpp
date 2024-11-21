
#include "Widget/CouchWidgetWin.h"
#include "Animation/WidgetAnimation.h"
#include "Arena/CouchGameManagerSubSystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UCouchWidgetWin::NativeConstruct()
{
	Super::NativeConstruct();
	if (OpenAnimation)
	{
		CheckLight();
		
		if (ConstructAnim)
		{
			FTimerHandle RoundTimerHandle;
			PlayAnimationForward(OpenAnimation);
			GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, [this]() { EndAnimation(); }, OpenAnimation->GetEndTime(), false);		
		}
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


void UCouchWidgetWin::CheckLight()
{
	int RoundNumber = 3;
	if(RoundNumber == 3)
	{
		Img_Light1->SetVisibility(ESlateVisibility::Hidden);
		Img_Light5->SetVisibility(ESlateVisibility::Hidden);
		
		Img_Light2->SetVisibility(ESlateVisibility::Visible);
		Img_Light3->SetVisibility(ESlateVisibility::Visible);
		Img_Light4->SetVisibility(ESlateVisibility::Visible);
	}
	else if(RoundNumber == 2)
	{
		Img_Light1->SetVisibility(ESlateVisibility::Hidden);
		Img_Light3->SetVisibility(ESlateVisibility::Hidden);
		Img_Light5->SetVisibility(ESlateVisibility::Hidden);
		
		Img_Light2->SetVisibility(ESlateVisibility::Visible);
		Img_Light4->SetVisibility(ESlateVisibility::Visible);
	}
	else if(RoundNumber == 4)
	{
		Img_Light3->SetVisibility(ESlateVisibility::Hidden);

		Img_Light1->SetVisibility(ESlateVisibility::Visible);
		Img_Light2->SetVisibility(ESlateVisibility::Visible);
		Img_Light4->SetVisibility(ESlateVisibility::Visible);
		Img_Light5->SetVisibility(ESlateVisibility::Visible);
	}
	else if(RoundNumber == 5)
	{
		Img_Light1->SetVisibility(ESlateVisibility::Visible);
		Img_Light2->SetVisibility(ESlateVisibility::Visible);
		Img_Light3->SetVisibility(ESlateVisibility::Visible);
		Img_Light4->SetVisibility(ESlateVisibility::Visible);
		Img_Light5->SetVisibility(ESlateVisibility::Visible);
	}
}

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
