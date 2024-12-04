#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CouchWidgetStartMenu.generated.h"

class UCouchCameraMove;
class UButton;

UCLASS()
class COUCHGAME_API UCouchWidgetStartMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnCameraTransitionEnd(bool Forward);
	
  
	void InitializeStartMenu();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPlayTransitionFinished"))
	void Receive_OnPlayTransitionFinished(bool Forward);


#pragma region Buttons
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Play;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Settings;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Credits;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Quit;
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayPressed"))
	void Receive_OnPlayPressed(bool Forward);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnSettingsPressed"))
	void Receive_OnSettingsPressed();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCreditsPressed"))
	void Receive_OnCreditsPressed();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnQuitPressed"))
	void Receive_OnQuitPressed();

private:
	UFUNCTION(BlueprintCallable)
	void OnPlayPressed();
	
	UFUNCTION(BlueprintCallable)
	void OnSettingsPressed();
  
	UFUNCTION(BlueprintCallable)
	void OnCreditsPressed();
  
	UFUNCTION(BlueprintCallable)
	void OnQuitPressed();

#pragma endregion
#pragma region Camera
public:
	TObjectPtr<ACameraActor> Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCouchCameraMove* CameraMove;
  


  
#pragma endregion

};
