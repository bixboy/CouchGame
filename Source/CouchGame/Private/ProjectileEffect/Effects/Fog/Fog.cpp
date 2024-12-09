#include "ProjectileEffect/Effects/Fog/Fog.h"

AFog::AFog()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
}

void AFog::UpdateFog(float Value, float MaxOpacity) const
{
	//if (MaterialInstance)
	//{
	//	MaterialInstance->SetScalarParameterValue(FName("Opacity"), FMath::Clamp(Value, 0, MaxOpacity));
	//}
}

