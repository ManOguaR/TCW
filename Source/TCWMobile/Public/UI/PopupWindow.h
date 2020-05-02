// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "HexUIButton.h"

#include "PopupWindow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPopupWindowSignature, UWidget*, contentWidget);

/**
 *
 */
UCLASS()
class TCWMOBILE_API UPopupWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	UPopupWindow(const FObjectInitializer& ObjectInitializer);

protected:
	void NativeDestruct() override;

	UFUNCTION()
		virtual void DisplaySelfEvent(UWidget* contentWidget);
	UFUNCTION()
		virtual void OnCloseButtonClickedInternal();

private:
	FScriptDelegate OnCloseWindowClicked;

public:
	UPROPERTY(BlueprintCallable, Category = "Events")
		FPopupWindowSignature OnDisplaySelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UHexUIButton* CloseWindowButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* DisplaySelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* ContentBorder;
};
