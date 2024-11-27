// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CouchWidgetStartMenu.h"

#include "Camera/CameraActor.h"
#include "Components/Button.h"
#include "Components/CouchCameraMove.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/GameplayStatics.h"

void UCouchWidgetStartMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()))
	{
		if (Camera = Cast<ACameraActor>(Actor); Camera)
		{
			if(UActorComponent* CameraComponent = Camera->GetComponentByClass(UCouchCameraMove::StaticClass()))
			{
				if (CameraMove = Cast<UCouchCameraMove>(CameraComponent); CameraMove)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, "Starting Menu");
				}
			}
		}
	}
	InitializeStartMenu();
}

void UCouchWidgetStartMenu::OnCameraTransitionEnd(bool Forward)
{
	Receive_OnPlayTransitionFinished(Forward);
}

void UCouchWidgetStartMenu::InitializeStartMenu()
{
	if (Btn_Play)
	{
		Btn_Play->OnPressed.AddDynamic(this, &UCouchWidgetStartMenu::OnPlayPressed);
	}
	if (Btn_Settings)
	{
		Btn_Settings->OnPressed.AddDynamic(this, &UCouchWidgetStartMenu::OnSettingsPressed);
	}
	if (Btn_Credits)
	{
		Btn_Settings->OnPressed.AddDynamic(this, &UCouchWidgetStartMenu::OnCreditsPressed);
	}
	if (Btn_Quit)
	{
		Btn_Quit->OnPressed.AddDynamic(this, &UCouchWidgetStartMenu::OnQuitPressed);
	}
	if (CameraMove)
	{
		CameraMove->TravelingEnd.AddDynamic(this, &UCouchWidgetStartMenu::OnCameraTransitionEnd);
	}
}

void UCouchWidgetStartMenu::OnPlayPressed()
{
	if (!CameraMove) return;
	CameraMove->StartCameraMove(true);
}

void UCouchWidgetStartMenu::OnSettingsPressed()
{
}

void UCouchWidgetStartMenu::OnCreditsPressed()
{
}

void UCouchWidgetStartMenu::OnQuitPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit, false);
}
