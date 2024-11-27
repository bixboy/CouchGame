
#include "Widget/CouchWidgetSelectionTeam.h"
#include "Components/Button.h"
#include "Components/Image.h"

bool UCouchWidgetSelectionTeam::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (Btn_Play)
	{
		Btn_Play->OnPressed.AddDynamic(this, &UCouchWidgetSelectionTeam::OnplayPressed);	
	}
	return true;
}

void UCouchWidgetSelectionTeam::OnplayPressed()
{
}

void UCouchWidgetSelectionTeam::SetArrowVisibility(ESlateVisibility NewVisibility)
{
	Img_LeftArrowPlayer1->SetVisibility(NewVisibility);
	Img_LeftArrowPlayer1_1->SetVisibility(NewVisibility);
	Img_LeftArrowPlayer1_2->SetVisibility(NewVisibility);
	Img_LeftArrowPlayer1_3->SetVisibility(NewVisibility);
	Img_RightArrowPlayer1->SetVisibility(NewVisibility);
	Img_RightArrowPlayer1_1->SetVisibility(NewVisibility);
	Img_RightArrowPlayer1_2->SetVisibility(NewVisibility);
	Img_RightArrowPlayer1_3->SetVisibility(NewVisibility);
}

void UCouchWidgetSelectionTeam::ChangeImage(UImage* TargetImage, UTexture2D* NewTexture)
{
	if (TargetImage && NewTexture)
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(NewTexture);

		TargetImage->SetBrush(NewBrush);
	}
}
