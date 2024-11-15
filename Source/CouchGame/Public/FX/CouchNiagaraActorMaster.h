
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchNiagaraActorMaster.generated.h"

class UNiagaraComponent;

UCLASS()
class COUCHGAME_API ACouchNiagaraActorMaster : public AActor
{
	GENERATED_BODY()

public:
	ACouchNiagaraActorMaster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	float TimeToDestroy = 0.f;
	
	void OnTimerEnd();
};
