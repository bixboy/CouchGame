// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CouchCraftingValidateItem.h"

#include "Components/BoxComponent.h"
#include "Crafting/CouchCraftingTable.h"


// Sets default values
ACouchCraftingValidateItem::ACouchCraftingValidateItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveBox"));
	RootComponent = Box;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ACouchCraftingValidateItem::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
	if (!CraftingTable) return;
	CraftingTable->CraftItem();
	if (CurrentPlayer) CurrentPlayer->AnimationManager->IsCheckingChef = !CurrentPlayer->AnimationManager->IsCheckingChef;
}

void ACouchCraftingValidateItem::BeginPlay()
{
	Super::BeginPlay();
	if (!CraftingTable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, this->GetName() + " is not set properly");
	}
}


