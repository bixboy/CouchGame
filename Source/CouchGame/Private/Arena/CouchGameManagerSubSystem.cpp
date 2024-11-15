
#include "Arena/CouchGameManagerSubSystem.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/CouchWidgetWin.h"

#pragma region Initialize

void UCouchGameManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("CouchGameManagerSubSystem initialized successfully!"));
}

void UCouchGameManagerSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UCouchGameManagerSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

#pragma endregion

void UCouchGameManagerSubSystem::SetupRounds(int RoundsNumber, float RoundDuration, FName Level, TSubclassOf<UCouchWidgetWin> Widget)
{
	MaxRounds = RoundsNumber;
	RoundDurationMinutes = RoundDuration;
	LevelName = Level;
	WinWidget = Widget;
	StartNewRound();
}

void UCouchGameManagerSubSystem::StartNewRound()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	CurrentRound++;

	// Démarre un timer de 3 minutes pour la manche
	float RoundDurationInSeconds = RoundDurationMinutes * 60.0f;
	GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &UCouchGameManagerSubSystem::OnRoundTimerEnd, RoundDurationInSeconds, false);
}

void UCouchGameManagerSubSystem::CheckRoundWinCondition(int TeamWin)
{
	TeamWin = FMath::Clamp(TeamWin, 0, 2);

	if (TeamWin == 1)
	{
		if (UCouchWidgetWin* Widget = CreateWidget<UCouchWidgetWin>(GetWorld(), WinWidget))
		{
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			Widget->ChangeWinnerText(FText::FromString("Team A wins the round!"));
			Widget->AddToViewport();
		}
		
		// L'équipe A gagne la manche
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Team A wins the round!"));
		TeamAPRoundWin++;
	}
	else if (TeamWin == 2)
	{
		if (UCouchWidgetWin* Widget = CreateWidget<UCouchWidgetWin>(GetWorld(), WinWidget))
		{
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			Widget->ChangeWinnerText(FText::FromString("Team B wins the round!"));
			Widget->AddToViewport();
		}
		
		// L'équipe B gagne la manche
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Team B wins the round!"));
		TeamBPRoundWin++;
	}
	else if (TeamWin == 0)
	{
		// No Team win
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("No Team Win!"));

		if (UCouchWidgetWin* Widget = CreateWidget<UCouchWidgetWin>(GetWorld(), WinWidget))
		{
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			Widget->ChangeWinnerText(FText::FromString("No Team wins the round!"));
			Widget->AddToViewport();
		}
	}

	// Condition de fin de jeu si le nombre maximum de manches est atteint
	if (CurrentRound > MaxRounds)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Game Finish!"));

		if (UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), WinWidget))
		{
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			WidgetInstance->AddToViewport();
		}
	}
}

void UCouchGameManagerSubSystem::OnRoundTimerEnd()
{
	if (Team1Health > Team2Health)
	{
		// Team 1 gagne la manche
		CheckRoundWinCondition(1);
	}
	else if (Team2Health > Team1Health)
	{
		// Team 2 gagne la manche
		CheckRoundWinCondition(2);
	}
	else
	{
		// Match null
	}
}

void UCouchGameManagerSubSystem::ResetRound()
{
	CurrentRound = 1;
	TeamAPRoundWin = 0;
	TeamBPRoundWin = 0;
}

void UCouchGameManagerSubSystem::UpdateCurrentLife(int CurrentTeam, float CurrentLife)
{
	if (CurrentTeam == 1)
	{
		// Vie de la team 1
		Team1Health = CurrentLife;
		if (Team1Health <= 0)
		{
			CheckRoundWinCondition(2);
		}
	}
	else if (CurrentTeam == 2)
	{
		// Vie de la team 2
		Team2Health = CurrentLife;
		if (Team2Health <= 0)
		{
			CheckRoundWinCondition(1);
		}
	}
}
