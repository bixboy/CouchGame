#include "Widget/CouchWidgetTimer.h"
#include "Components/TextBlock.h"

FText UCouchWidgetTimer::UpdateTimer(float Time)
{
	int32 Minutes = FMath::FloorToInt(Time / 60);    // Minutes
	int32 Seconds = FMath::FloorToInt(Time) % 60;    // Secondes

	FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	Txt_Timer->SetText(FText::FromString(TimerText));
	
	if (Time <= TimeWarning)
	{
		StartAnimWarning();
	}

	return FText::FromString(TimerText);
}
