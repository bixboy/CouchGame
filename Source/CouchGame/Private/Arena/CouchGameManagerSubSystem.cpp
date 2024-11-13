
#include "Arena/CouchGameManagerSubSystem.h"

#include "Blueprint/UserWidget.h"

void UCouchGameManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentRound = 1;
	MaxRounds = 3;
}

void UCouchGameManagerSubSystem::CheckRoundWinCondition(int TeamWin)
{
	TeamWin = FMath::Clamp(TeamWin, 1, 2);

	if (TeamWin == 2)
	{
		// L'équipe B gagne la manche
		UE_LOG(LogTemp, Log, TEXT("Team B wins the round!"));
		TeamBPRoundWin++;
		CurrentRound++;
	}
	else if (TeamWin == 1)
	{
		// L'équipe A gagne la manche
		UE_LOG(LogTemp, Log, TEXT("Team A wins the round!"));
		TeamAPRoundWin++;
		CurrentRound++;
	}

	// Condition de fin de jeu si le nombre maximum de manches est atteint
	if (CurrentRound > MaxRounds)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Finish!"));
		
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), WinWidget);
		if (WidgetInstance)
			WidgetInstance->AddToViewport();
	}
}

void UCouchGameManagerSubSystem::ResetRound()
{
	CurrentRound = 1;
	TeamAPRoundWin = 0;
	TeamBPRoundWin = 0;
}
