
#pragma once

#include "CoreMinimal.h"
#include "LocalMultiplayerSettings.h"
#include "Characters/CouchCharacter.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CouchGameManagerSubSystem.generated.h"

class UCouchWidgetTimer;
class UCouchWidgetWin;

USTRUCT(Blueprintable, BlueprintType)
struct FCouchPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Couch Player")
	int PlayerIndex = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Couch Player")
	TEnumAsByte<EAutoReceiveInput::Type> InputType = EAutoReceiveInput::Type::Disabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Couch Player")
	int TeamIndex = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Couch Player")
	int CardIndex = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Couch Player")
	TObjectPtr<USkeletalMesh> SkeletalMeshAsset;
};


UCLASS(Abstract, Blueprintable, BlueprintType)
class COUCHGAME_API UCouchGameManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// Initialisation du système
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

#pragma region Teams
public:	
	// Gestion des équipes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int32 TeamAPRoundWin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int32 TeamBPRoundWin;

	float Team1Health = 0.f;
	float Team2Health = 0.f;

	void UpdateCurrentLife(int CurrentTeam, float CurrentLife);

#pragma endregion	

#pragma region Rounds
protected:	
	// Gestion des manches
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	int32 CurrentRound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	int32 MaxRounds;

	UPROPERTY()
	TArray<int> RoundsWinners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	TSubclassOf<UCouchWidgetWin> WinWidget;

	// Timer de manches
	FTimerHandle RoundTimerHandle;
	float CurrentRoundTimer;
	bool EndMatch = false;

	FTimerHandle UiTimerHandle;
	void DecrementTimer();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	float RoundDurationMinutes  = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	FName LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	TSoftObjectPtr<UWorld> LevelPtr;
	

public:	
	// Fonctions pour gérer les manches
	UFUNCTION(BlueprintCallable)
	void StartNewRound();
	UFUNCTION(BlueprintCallable)
	void CheckRoundWinCondition(int TeamWin);
	UFUNCTION(BlueprintCallable)
	void SetupRounds(int RoundsNumber, float RoundDuration, FName Level, TSubclassOf<UCouchWidgetWin> Widget);

	UFUNCTION(BlueprintCallable)
	void OpenUi(TSubclassOf<UCouchWidgetWin> Widget, FText Text);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTime() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetMaxRound();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentRound();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetEndMatch();

	TArray<int> GetRoundsWinners();

	void ResetRound();
	void OnRoundTimerEnd();

	void ReturnToMenu();
	
#pragma endregion


#pragma region Local Multiplayer
public:
	UPROPERTY(EditAnywhere, Category = "Characters")
	TSubclassOf<ACouchCharacter> CouchCharacterClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplayer")
	TArray<FCouchPlayerData> Players;
	UFUNCTION(BlueprintCallable)
	void AddPlayer(FCouchPlayerData Data);
	UFUNCTION(BlueprintCallable)
	void ResetPlayers();
	// void RemovePlayer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplayer")
	ELocalMultiplayerInputMappingType MappingType = ELocalMultiplayerInputMappingType::Menu;

	UFUNCTION(BlueprintCallable)
	void SwitchMappingType();
#pragma endregion
};
