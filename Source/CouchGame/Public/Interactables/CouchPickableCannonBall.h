// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchPickableMaster.h"
#include "GameFramework/Actor.h"
#include "CouchPickableCannonBall.generated.h"

UCLASS()
class COUCHGAME_API ACouchPickableCannonBall : public ACouchPickableMaster
{
	GENERATED_BODY()

public:
	ACouchPickableCannonBall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Top;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Down;

	virtual void InteractWithObject_Implementation(ACouchInteractableMaster* interactable) override;
};
