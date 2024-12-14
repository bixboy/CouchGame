#include "Widget/CouchCraftValidateWidget.h"

#include "Components/WidgetComponent.h"

ACouchCraftValidateWidget::ACouchCraftValidateWidget()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(RootComponent);
}

bool ACouchCraftValidateWidget::GetIsValid() const
{
	return bIsValid;
}

void ACouchCraftValidateWidget::SetIsValid(bool val)
{
	bIsValid = val;
	if (bIsValid)
	{
		WidgetComponent->SetWidgetClass(ValidateWidget);
	}
	else
	{
		WidgetComponent->SetWidgetClass(NoValidateWidget);
	}
}

