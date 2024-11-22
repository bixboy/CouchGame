#include "Crafting/CouchCraftingValidateItem.h"
#include "Components/BoxComponent.h"
#include "Crafting/CouchCraftingTable.h"
#include "Widget/CouchWidget3D.h"
#include "Widget/CouchWidgetSpawn.h"

ACouchCraftingValidateItem::ACouchCraftingValidateItem()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetSpawner = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgetSpawner"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveBox"));
	RootComponent = Box;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	WidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetPose"));
	WidgetPose->SetupAttachment(RootComponent);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACouchCraftingValidateItem::OnCharacterBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACouchCraftingValidateItem::OnCharacterEndOverlap);
}

void ACouchCraftingValidateItem::BeginPlay()
{
	Super::BeginPlay();
	if (!CraftingTable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, this->GetName() + " is not set properly");
	}
}

void ACouchCraftingValidateItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CraftingTable) return;
	if (IsWidgetActive && !CraftingTable->IsCraftingPossible())
	{
		WidgetSpawner->DestroyWidget();
		IsWidgetActive = false;
	}
}

void ACouchCraftingValidateItem::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
	if (!CraftingTable) return;
	CraftingTable->CraftItem();
	if (CurrentPlayer) CurrentPlayer->AnimationManager->IsCheckingChef = !CurrentPlayer->AnimationManager->IsCheckingChef;
}


void ACouchCraftingValidateItem::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CraftingTable) return;
	if (CraftingTable->IsCraftingPossible() && WidgetInteract)
	{
		WidgetSpawner->SpawnWidget(WidgetInteract, WidgetPose);
		IsWidgetActive = true;
	}
}

void ACouchCraftingValidateItem::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	WidgetSpawner->DestroyWidget();
	IsWidgetActive = false;
}


