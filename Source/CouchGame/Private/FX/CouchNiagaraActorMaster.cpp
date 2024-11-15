
#include "FX/CouchNiagaraActorMaster.h"
#include "NiagaraComponent.h"

ACouchNiagaraActorMaster::ACouchNiagaraActorMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	RootComponent = NiagaraComponent;
}

void ACouchNiagaraActorMaster::BeginPlay()
{
	Super::BeginPlay();

	if(TimeToDestroy > 0)
	{
		FTimerHandle RoundTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &ACouchNiagaraActorMaster::OnTimerEnd, TimeToDestroy, false);	
	}
}

void ACouchNiagaraActorMaster::OnTimerEnd()
{
	Destroy();
}

