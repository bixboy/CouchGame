
#include "Lobby/CouchTeamCard.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACouchTeamCard::ACouchTeamCard()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card"));
}

void ACouchTeamCard::BeginPlay()
{
	Super::BeginPlay();
	PlayerNumber = 3;
	SpawnCharacter();
}

void ACouchTeamCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACouchTeamCard::SpawnCharacter()
{
	if (CurrentPlayer < PlayerNumber)
	{
		CurrentPlayer ++;
		
		FVector Origin;
		FVector BoxExtent;
		GetActorBounds(false, Origin, BoxExtent);
		FVector RandomPosition = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent);
		
		FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), FVector(RandomPosition.X, RandomPosition.Y, RandomPosition.Z + 1200));
		ACouchCharacter* Character = GetWorld()->SpawnActor<ACouchCharacter>(CharacterToSpawn, SpawnTransform);
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACouchTeamCard::SpawnCharacter, 1.0f, false);
	}
}

