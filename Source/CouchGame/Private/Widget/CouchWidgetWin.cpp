
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
	CheckLight();
	TurnOnLight();
	if (OpenAnimation)
	{
		if (ConstructAnim)
		{
			FTimerHandle RoundTimerHandle;
			PlayAnimation(OpenAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, PlayRate);
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
	if (GameManager && !GameManager->GetEndMatch())
	{
		GameManager->StartNewRound();
	}
	else if (GameManager && GameManager->GetEndMatch())
	{
		GameManager->ReturnToMenu();
	}
}

#pragma endregion


void UCouchWidgetWin::TurnOnLight()
{
	UCouchGameManagerSubSystem* GameManager = GetWorld()->GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	if (!GameManager) return;

	TArray<UImage*> AllLanterns = { Img_Light1, Img_Light2, Img_Light3, Img_Light4, Img_Light5 };

	TArray<UImage*> VisibleLanterns;
	for (UImage* Lantern : AllLanterns)
	{
		if (Lantern && Lantern->Visibility == ESlateVisibility::Visible)
		{
			VisibleLanterns.Add(Lantern);
		}
	}

	const TArray<int>& RoundWinners = GameManager->GetRoundsWinners();
	
	// Mettre à jour les lanternes en fonction de l'historique
	for (int i = 0; i < VisibleLanterns.Num(); ++i)
	{
		if (i < RoundWinners.Num())
		{
			// Si une équipe a gagné cette manche, définir la couleur correspondante
			if (RoundWinners[i] == 1) // Team A
			{
				VisibleLanterns[i]->SetBrushFromTexture(LightOn);
			}
			else if (RoundWinners[i] == 2) // Team B
			{
				VisibleLanterns[i]->SetBrushFromTexture(LightRed);
			}
			else if (RoundWinners[i] == 0)
			{
				VisibleLanterns[i]->SetBrushFromTexture(LightOff);
			}
		}
		else
		{
			// Lanternes restantes (pas encore jouées)
			VisibleLanterns[i]->SetBrushFromTexture(LightOff);
			VisibleLanterns[i]->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UCouchWidgetWin::CheckLight()
{
	if (UCouchGameManagerSubSystem* GameManager = GetWorld()->GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>())
	{
		int RoundNumber = GameManager->GetMaxRound();
		if (RoundNumber == 1)
		{
			if (Img_Light1) Img_Light1->SetVisibility(ESlateVisibility::Hidden);
			if (Img_Light5) Img_Light5->SetVisibility(ESlateVisibility::Hidden);
			if (Img_Light2) Img_Light2->SetVisibility(ESlateVisibility::Hidden);
			if (Img_Light4) Img_Light4->SetVisibility(ESlateVisibility::Hidden);
			
			if (Img_Light3) Img_Light3->SetVisibility(ESlateVisibility::Visible);
			
		}
		else if(RoundNumber == 3)
		{
			if (Img_Light1) Img_Light1->SetVisibility(ESlateVisibility::Hidden);
			if (Img_Light5) Img_Light5->SetVisibility(ESlateVisibility::Hidden);
			
			if (Img_Light2) Img_Light2->SetVisibility(ESlateVisibility::Visible);
			if (Img_Light3) Img_Light3->SetVisibility(ESlateVisibility::Visible);
			if (Img_Light4) Img_Light4->SetVisibility(ESlateVisibility::Visible);
		}
		else if(RoundNumber == 5)
		{
			if (Img_Light1) Img_Light1->SetVisibility(ESlateVisibility::Visible);
			if (Img_Light2) Img_Light2->SetVisibility(ESlateVisibility::Visible);
			if (Img_Light3) Img_Light3->SetVisibility(ESlateVisibility::Visible);
			if (Img_Light4) Img_Light4->SetVisibility(ESlateVisibility::Visible);
			if (Img_Light5) Img_Light5->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UCouchWidgetWin::EndAnimation()
{
	PlayAnimationForward(TextAppearAnimation);
	StartConfetti();

	FTimerHandle RoundTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &UCouchWidgetWin::OnNewRoundPressed, TimeToNewRound, false);
}

void UCouchWidgetWin::StartConfetti()
{
	NSWidget_0->ActivateSystem(true);
	NSWidget_1->ActivateSystem(true);
	NSWidget_2->ActivateSystem(true);
}
