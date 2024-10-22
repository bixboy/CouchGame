// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame/Public/CouchCharacter.h"


// Sets default values
ACouchCharacter::ACouchCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACouchCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACouchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ACouchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACouchCharacter::GetOrientX() const
{
	return OrientX;
}

void ACouchCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ACouchCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = (OrientX == 1.f) ? 0 : 180;
	GetMesh()->SetRelativeRotation(Rotation);
}

