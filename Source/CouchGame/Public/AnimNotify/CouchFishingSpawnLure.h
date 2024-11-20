#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CouchFishingSpawnLure.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifiedSignature);
UCLASS()
class COUCHGAME_API UCouchFishingSpawnLure : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnNotifiedSignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
