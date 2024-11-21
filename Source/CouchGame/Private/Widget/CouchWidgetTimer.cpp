#include "Widget/CouchWidgetTimer.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

FText UCouchWidgetTimer::UpdateTimer(float time)
{
	FString TimerText = UKismetStringLibrary::TimeSecondsToString(time);
	Txt_Timer->SetText(FText::FromString(TimerText));
	return FText::FromString(TimerText);
}
