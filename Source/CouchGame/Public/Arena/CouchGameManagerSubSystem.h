
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CouchGameManagerSubSystem.generated.h"

class UCouchWidgetWin;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	TSubclassOf<UCouchWidgetWin> WinWidget;

	// Timer de manches
	FTimerHandle RoundTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	float RoundDurationMinutes  = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	FName LevelName;

public:	
	// Fonctions pour gérer les manches
	UFUNCTION(BlueprintCallable)
	void StartNewRound();
	UFUNCTION(BlueprintCallable)
	void CheckRoundWinCondition(int TeamWin);
	UFUNCTION(BlueprintCallable)
	void SetupRounds(int RoundsNumber, float RoundDuration, FName Level, TSubclassOf<UCouchWidgetWin> Widget);

	void ResetRound();
	void OnRoundTimerEnd();
	
#pragma endregion
	
};
