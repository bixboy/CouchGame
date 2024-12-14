#include "Widget/CouchWidgetStartMenu.h"

#include "Camera/CameraActor.h"
#include "Components/Button.h"
#include "Components/CouchCameraMove.h"
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
	FInternationalization::Get().LoadAllCultureData();
	FTextLocalizationManager::Get().RefreshResources();
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
	if (!CameraMove)
	{
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
	}
	Receive_OnPlayPressed(true);
	CameraMove->StartCameraMove(true);
}


void UCouchWidgetStartMenu::OnSettingsPressed()
{
	Receive_OnSettingsPressed();
}


void UCouchWidgetStartMenu::OnCreditsPressed()
{
	Receive_OnCreditsPressed();
}


void UCouchWidgetStartMenu::OnQuitPressed()
{
	Receive_OnQuitPressed();
}
