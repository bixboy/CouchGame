// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/CouchShootNotify.h"

#include "Interactables/CouchCatapult.h"

void UCouchShootNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACouchCatapult* Catapult = Cast<ACouchCatapult>(MeshComp->GetOwner()))
	{
		Catapult->Fire();
	}
}
