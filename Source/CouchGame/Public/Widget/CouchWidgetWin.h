
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraSystemWidget.h"
#include "CouchWidgetWin.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

UCLASS()
class COUCHGAME_API UCouchWidgetWin : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	

#pragma region Animation
private:
	void EndAnimation();
	void StartConfetti();

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnimation;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> TextAppearAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidget_0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidget_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidget_2;
	
#pragma endregion 	

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
