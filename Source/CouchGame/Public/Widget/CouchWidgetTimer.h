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
	
	UPROPERTY(meta = (BindWidgetAnim), Transient, BlueprintReadWrite)
	TObjectPtr<UWidgetAnimation> WarningAnimation;

	UFUNCTION(BlueprintCallable)
	FText UpdateTimer(float Time);

private:
	UPROPERTY(EditAnywhere)
	float TimeWarning = 0.30f;
};
