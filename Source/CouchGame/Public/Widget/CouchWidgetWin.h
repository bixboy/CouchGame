
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CouchWidgetWin.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

UCLASS()
class COUCHGAME_API UCouchWidgetWin : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	virtual bool Initialize() override;

#pragma region Text
public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Txt_Winner;
	UFUNCTION(BlueprintCallable)
	void ChangeWinnerText(FText Winner);

#pragma endregion	

#pragma region Buttons
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_NewRound;

private:
	UFUNCTION()
	void OnNewRoundPressed();

#pragma endregion	
};
