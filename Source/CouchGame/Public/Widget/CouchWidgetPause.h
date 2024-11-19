#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CouchWidgetPause.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class COUCHGAME_API UCouchWidgetPause : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnimation;

	UFUNCTION()
	void EndAnimation(bool Pause);

private:
	UPROPERTY(EditAnywhere,Category = DefaultValue)
	FName MenuName;

#pragma region Text
public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> Txt_Pause;

#pragma endregion	

#pragma region Buttons
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Quit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Resume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Menu;

private:
	UFUNCTION()
	void OnQuitPressed();

	UFUNCTION()
	void OnResumePressed();

	UFUNCTION()
	void OnMenuPressed();

#pragma endregion	
};
