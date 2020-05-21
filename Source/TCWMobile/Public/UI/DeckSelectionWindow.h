// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeckSelectionWindow.generated.h"

DECLARE_DYNAMIC_DELEGATE(FDeckSelectionEvent);

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
	//virtual void OnCloseButtonClickedInternal() override;

//private:
//	FScriptDelegate OnAcceptButtonClicked;
//	FScriptDelegate OnCancelButtonClicked;

public:
	UPROPERTY(EditAnywhere, Category = "Events")
		FDeckSelectionEvent OnWindowClosed;

};
