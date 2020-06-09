// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeckSelectionWindow.generated.h"

class UHexUIButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeckSelectionEvent);
DECLARE_DYNAMIC_DELEGATE(FDeckSelectionResponseEvent);

/**
 * 
 */
UCLASS()
class TCWMOBILE_API UDeckSelectionWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDeckSelectionWindow(const FObjectInitializer& ObjectInitializer);

protected:
	void NativeDestruct() override;

	UFUNCTION()
		void DisplayWindow();
	UFUNCTION()
		void OnCloseButtonClickedInternal();

private:
	FScriptDelegate OnCloseWindowClicked;

public:
	UPROPERTY(EditAnywhere, Category = "Events")
		FDeckSelectionResponseEvent OnWindowClosed;

	UPROPERTY(BlueprintCallable, Category = "Events")
		FDeckSelectionEvent OnDisplayWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UHexUIButton* CloseWindowButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* DisplaySelf;

private:
	void OnCloseButtonClicked_Delayed();
	void OnCloseButtonClicked_Callback();

	void PopulateDeckList();
};
