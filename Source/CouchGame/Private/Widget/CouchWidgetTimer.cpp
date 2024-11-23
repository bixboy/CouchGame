#include "Widget/CouchWidgetTimer.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

FText UCouchWidgetTimer::UpdateTimer(float time)
{
	int32 Minutes = FMath::FloorToInt(time / 60);    // Minutes
	int32 Seconds = FMath::FloorToInt(time) % 60;    // Secondes

	FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	Txt_Timer->SetText(FText::FromString(TimerText));

	return FText::FromString(TimerText);
}
