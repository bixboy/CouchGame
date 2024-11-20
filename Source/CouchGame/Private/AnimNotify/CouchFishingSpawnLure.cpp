
#include "AnimNotify/CouchFishingSpawnLure.h"
#include "Characters/CouchCharacter.h"
#include "Interactables/CouchFishingRod.h"

void UCouchFishingSpawnLure::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ACouchCharacter* Player = Cast<ACouchCharacter>(MeshComp->GetOwner()))
	{
		Player->GetFishingRod()->SpawnLure();
	}
}
