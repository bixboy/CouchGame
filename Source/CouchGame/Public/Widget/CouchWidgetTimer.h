#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CouchWidgetTimer.generated.h"

class UTextBlock;

UCLASS()
class COUCHGAME_API UCouchWidgetTimer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> Txt_Timer;

	UFUNCTION(BlueprintCallable)
	FText UpdateTimer(float time);
};
