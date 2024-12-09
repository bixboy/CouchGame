
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraSystemWidget.h"
#include "CouchWidgetWin.generated.h"

class UImage;
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

private:
	float TimeToNewRound = 5.f;

#pragma region Light

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Light1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Light2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Light3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Light4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Light5;

	void TurnOnLight();

private:
	void CheckLight();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> LightOn;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> LightOff;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> LightRed;

#pragma endregion	

#pragma region Animation
private:
	void EndAnimation();
	void StartConfetti();

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient, BlueprintReadWrite)
	TObjectPtr<UWidgetAnimation> OpenAnimation;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> TextAppearAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidget_0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidget_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidget_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ConstructAnim = true;
	
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
