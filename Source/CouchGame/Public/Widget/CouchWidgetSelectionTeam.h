#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CouchWidgetSelectionTeam.generated.h"

class UImage;
class UButton;

UCLASS()
class COUCHGAME_API UCouchWidgetSelectionTeam : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

#pragma region Image
public:
	// Arrow Left
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_LeftArrowPlayer1;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_LeftArrowPlayer1_1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_LeftArrowPlayer1_2;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_LeftArrowPlayer1_3;

	// Arrow Right
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_RightArrowPlayer1;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_RightArrowPlayer1_1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_RightArrowPlayer1_2;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_RightArrowPlayer1_3;

	//Player Left / right
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_LeftPlayer1;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_LeftPlayer2;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_RightPlayer1;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Img_RightPlayer2;

#pragma endregion	

#pragma region Buttons
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Play;

private:
	UFUNCTION()
	void OnplayPressed();
	UFUNCTION(BlueprintCallable, Category = "SelectionTeam")
	void SetArrowVisibility(ESlateVisibility NewVisibility);
	UFUNCTION(BlueprintCallable, Category = "SelectionTeam")
	void ChangeImage(UImage* TargetImage, UTexture2D* NewTexture);

#pragma endregion	
};
