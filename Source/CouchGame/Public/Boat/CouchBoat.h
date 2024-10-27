#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchBoat.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class COUCHGAME_API ACouchBoat : public AActor
{
	GENERATED_BODY()

public:
	// Constructeur
	ACouchBoat();

	// Appelé au début du jeu ou lors de la spawn de l'acteur
	virtual void BeginPlay() override;

	// Fonctions de gestion des dégâts/réparations
	void BoatDamage(float DamageAmount);
	void BoatRepair(float HealAmount);
	float GetBoatLife() const;

private:
	// Composants
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BoatMesh;

	// Vie du bateau
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	float BoatStartLife = 100.f;

	UPROPERTY()
	float BoatLife;
};
