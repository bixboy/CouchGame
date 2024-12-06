// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersMenu/CouchCharacterMenu.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/CouchCharacterInputData.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/CouchCharacterLobby.h"


class ULocalMultiplayerSubsystem;
// Sets default values
ACouchCharacterMenu::ACouchCharacterMenu()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACouchCharacterMenu::InitMenuCharacter(UCouchCharacterLobby* In_Lobby)
{
	Lobby = In_Lobby;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Lobby");
}

// Called when the game starts or when spawned
void ACouchCharacterMenu::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(this->GetController());
}


void ACouchCharacterMenu::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ACouchCharacterMenu::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;
	
	BindInputActions(EnhancedInputComponent);
}

void ACouchCharacterMenu::SetupMappingContextIntoController() const
{
	if (!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSystem) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

void ACouchCharacterMenu::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!InputData) return;
	if(InputData->InputActionNavigate)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionNavigate,
			ETriggerEvent::Started,
			this,
			&ACouchCharacterMenu::OnInputNavigate
		);
	}
	if (InputData->InputActionValidate)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionValidate,
			ETriggerEvent::Started,
			this,
			&ACouchCharacterMenu::OnInputValidate
		);
	}
	if (InputData->InputActionCancel)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionCancel,
			ETriggerEvent::Started,
			this,
			&ACouchCharacterMenu::OnInputCancel
		);
	}
}

int ACouchCharacterMenu::GetPlayerIndex() const
{
	if (!PlayerController) return -1;

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer) return -1;

	return LocalPlayer->GetLocalPlayerIndex();
	// if (!PlayerController) return -1;
	//
	// // Récupérer l'identifiant de l'appareil utilisé par le joueur
	// const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	// return LocalPlayer->GetLocalPlayerIndex();
}


void ACouchCharacterMenu::OnInputNavigate(const FInputActionValue& InputActionValue)
{
	FVector2D InputDirection = InputActionValue.Get<FVector2D>();
	if (!Lobby)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Lobby not set");
		return;
	}
	InputDirection.Normalize();
	
	if (InputDirection.IsNearlyZero(0.3f))
	{
		return;
	}
	
	const int PlayerIndex = GetPlayerIndex();
	if (FMath::Abs(InputDirection.Y) > FMath::Abs(InputDirection.X))
	{
		// Axe vertical dominant
		if (InputDirection.Y > 0) // Haut
		{
			Lobby->Receive_Up(PlayerIndex);
		}
		else // Bas
		{
			Lobby->Receive_Down(PlayerIndex);
		}
	}
	else
	{
		// Axe horizontal dominant
		if (InputDirection.X > 0) // Droite
		{
			Lobby->Receive_Right(PlayerIndex);
		}
		else // Gauche
		{
			Lobby->Receive_Left(PlayerIndex);
		}
	}
}

void ACouchCharacterMenu::OnInputValidate()
{
	if (!Lobby) return;
	const int PlayerIndex = GetPlayerIndex();
	Lobby->Receive_Validate(PlayerIndex);
}

void ACouchCharacterMenu::OnInputCancel()
{
	if (!Lobby) return;
	const int PlayerIndex = GetPlayerIndex();
	Lobby->Receive_Cancel(PlayerIndex);
}

