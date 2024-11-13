
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CouchGameManagerSubSystem.generated.h"

UCLASS()
class COUCHGAME_API UCouchGameManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Initialisation du système
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Gestion des équipes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int32 TeamAPRoundWin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int32 TeamBPRoundWin;

	// Gestion des manches
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	int32 CurrentRound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	int32 MaxRounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
	TSubclassOf<UUserWidget> WinWidget;

	// Fonctions pour gérer les manches
	void CheckRoundWinCondition(int TeamWin);
	void ResetRound();
	
};
