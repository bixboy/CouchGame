#pragma once

#include "CoreMinimal.h"
#include "CouchWidget3D.h"
#include "CouchCraftValidateWidget.generated.h"

class UWidgetComponent;

UCLASS()
class COUCHGAME_API ACouchCraftValidateWidget : public ACouchWidget3D
{
	GENERATED_BODY()

public:
	ACouchCraftValidateWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	UFUNCTION(Blueprintable, BlueprintPure)
	bool GetIsValid() const;
	
	void SetIsValid(bool val);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ValidateWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> NoValidateWidget;
	
	bool bIsValid = false;
};
