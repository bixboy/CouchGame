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
	bool Initialize() override;

private:
	UPROPERTY(EditAnywhere,Category = DefaultValue)
	FName LobbyName;

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
	TObjectPtr<UButton> Btn_Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Play;

private:
	UFUNCTION()
	void OnQuitPressed();

	UFUNCTION()
	void OnOptionPressed();

	UFUNCTION()
	void OnPlayPressed();

#pragma endregion	
};
