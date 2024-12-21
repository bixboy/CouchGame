
#include "Arena/CouchGameManagerSubSystem.h"
#include "Blueprint/UserWidget.h"
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
	EndMatch = false;
	StartNewRound();
	ShowVictoryScreen = false;
}

void UCouchGameManagerSubSystem::OpenUi(TSubclassOf<UCouchWidgetWin> Widget, FText Text)
{
	if (UCouchWidgetWin* WidgetRef = CreateWidget<UCouchWidgetWin>(GetWorld(), WinWidget); WidgetRef)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetRef->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
		
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		WidgetRef->ChangeWinnerText(Text);
		WidgetRef->TurnOnLight();
		WidgetRef->PlayRate = 0.05f;
		WidgetRef->AddToViewport();
	}
}

#pragma region Rounds

//Start Round
void UCouchGameManagerSubSystem::StartNewRound()
{
	 if (!LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, LevelName, false);
	}
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	CurrentRound++;
	
	// Démarre un timer de 3 minutes pour la manche
	int Hours = FMath::FloorToInt(RoundDurationMinutes);
	float DecimalPart = RoundDurationMinutes - Hours;
	CurrentRoundTimer = (Hours * 60.0f) + (DecimalPart * 100.f);
	
	GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &UCouchGameManagerSubSystem::OnRoundTimerEnd, CurrentRoundTimer, false);
	
	GetWorld()->GetTimerManager().ClearTimer(UiTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(UiTimerHandle, this, &UCouchGameManagerSubSystem::DecrementTimer, 1.0f, true);
}

void UCouchGameManagerSubSystem::CheckRoundWinCondition(int TeamWin)
{
	if (EndMatch) return;
	TeamWin = FMath::Clamp(TeamWin, 0, 2);
	int RoundsToWin = (MaxRounds / 2) + 1;
	Team1WinTheGame = false;
	if (TeamWin == 1)
	{
		// L'équipe A gagne la manche
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Aoi Kumo wins the round!"));
		TeamAPRoundWin++;
		RoundsWinners.Add(1);
		
		if (TeamAPRoundWin >= RoundsToWin || CurrentRound == MaxRounds)
		{
			EndMatch = true;
			OpenUi(WinWidget, FText::FromString("Aoi Kumo wins the game!"));
			Team1WinTheGame = true;
			ShowVictoryScreen = true;
			return;
		}
		
		OpenUi(WinWidget, FText::FromString("Aoi Kumo wins the round!"));
	}
	else if (TeamWin == 2)
	{
		// L'équipe B gagne la manche
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Akai Taiyo wins the round!"));
		TeamBPRoundWin++;
		RoundsWinners.Add(2);

		if (TeamBPRoundWin >= RoundsToWin || CurrentRound == MaxRounds)
		{
			EndMatch = true;
			OpenUi(WinWidget, FText::FromString("Akai Taiyo wins the game!"));
			Team1WinTheGame = false;
			ShowVictoryScreen = true;
			return;
		}
		
		OpenUi(WinWidget, FText::FromString("Akai Taiyo wins the round!"));
	}
	else if (TeamWin == 0)
	{
		// No Team win
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("No Team Win!"));
		RoundsWinners.Add(0);
		if (CurrentRound == MaxRounds)
		{
			EndMatch = true;
			OpenUi(WinWidget, FText::FromString("No Team wins this game!"));
			return;
		}
		OpenUi(WinWidget, FText::FromString("No Team wins the round!"));
	}

	if (TeamAPRoundWin > MaxRounds / 2 || TeamBPRoundWin > MaxRounds / 2)
	{
		EndMatch = true;
		FString WinningTeamMessage = TeamAPRoundWin > TeamBPRoundWin 
			? "Aoi Kumo wins the game!" 
			: "Akai Taiyo wins the game!";
		OpenUi(WinWidget, FText::FromString(WinningTeamMessage));
		return;
	}
}

void UCouchGameManagerSubSystem::OnRoundTimerEnd()
{
	if (Team1Health > Team2Health && !EndMatch)
	{
		// Team 1 gagne la manche
		CheckRoundWinCondition(1);
	}
	else if (Team2Health > Team1Health && !EndMatch)
	{
		// Team 2 gagne la manche
		CheckRoundWinCondition(2);
	}
	else if(!EndMatch)
	{
		// Match null
		CheckRoundWinCondition(0);
	}
}

void UCouchGameManagerSubSystem::ResetRound()
{
	CurrentRound = 0;
	TeamAPRoundWin = 0;
	TeamBPRoundWin = 0;
	RoundsWinners.Reset();
	GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(UiTimerHandle);
}

// Update Life
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

void UCouchGameManagerSubSystem::ReturnToMenu()
{
	ResetRound();
	SwitchMappingType();
	UGameplayStatics::OpenLevel(this, "LevelLobby", false);
}

// Update Timer
void UCouchGameManagerSubSystem::DecrementTimer()
{
	if (CurrentRoundTimer > 0)
	{
		CurrentRoundTimer--;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(UiTimerHandle);
	}
}

// Get Time
float UCouchGameManagerSubSystem::GetTime() const
{
	return CurrentRoundTimer;
}

// Get Max Round
int UCouchGameManagerSubSystem::GetMaxRound()
{
	return MaxRounds;
}

// Get Current Round
int UCouchGameManagerSubSystem::GetCurrentRound()
{
	return CurrentRound;
}

bool UCouchGameManagerSubSystem::GetEndMatch()
{
	return EndMatch;
}

// Get Rounds Winners
TArray<int> UCouchGameManagerSubSystem::GetRoundsWinners()
{
	return RoundsWinners;
}

#pragma endregion

#pragma region Local Multiplayer
void UCouchGameManagerSubSystem::AddPlayer(FCouchPlayerData Data)
{
	Players.Add(Data);
}

void UCouchGameManagerSubSystem::ResetPlayers()
{
	Players.Reset();
}

// void UCouchGameManagerSubSystem::AddPlayer()
// {
// 	NbPlayersTeam1 = FMath::Clamp( NbPlayersTeam1+1, 0, 4);
// }
//
// void UCouchGameManagerSubSystem::RemovePlayer()
// {
// 	NbPlayersTeam1 = FMath::Clamp( NbPlayersTeam1-1, 0, 4);
// }



void UCouchGameManagerSubSystem::SwitchMappingType()
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::Menu :
		{
			MappingType = ELocalMultiplayerInputMappingType::InGame;
			break;
		}
	case ELocalMultiplayerInputMappingType::InGame:
		{
			MappingType = ELocalMultiplayerInputMappingType::Menu;
			break;
		}
	default:
		break;
	}
}
#pragma endregion