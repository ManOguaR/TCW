// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UI/PopupWindow.h"
#include "DialogWindow.generated.h"

UENUM()
enum class FDialogWindowResponse : uint8
{
	DialogWindow_Unknown,
	DialogWindow_OK,
	DialogWindow_Yes,
	DialogWindow_Accept,
	DialogWindow_No,
	DialogWindow_Cancel
};

UENUM()
enum class FDialogButtonsButtons : uint8
{
	DialogButtons_OK,
	DialogButtons_OKCancel,
	DialogButtons_Accept,
	DialogButtons_AcceptCancel,
	DialogButtons_YesNo,
	DialogButtons_Cancel
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDialogWindowEvent, FDialogButtonsButtons, buttons, FText, message);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDialogWindowResponseEvent, FDialogWindowResponse, response);

/**
 *
 */
UCLASS()
class TCWMOBILE_API UDialogWindow : public UPopupWindow
{
	GENERATED_BODY()

public:
	UDialogWindow(const FObjectInitializer& ObjectInitializer);

protected:
	void NativeDestruct() override;
	virtual void OnCloseButtonClickedInternal() override;

	UFUNCTION()
		virtual void ShowDialog(FDialogButtonsButtons buttons, FText message);

	UFUNCTION()
		virtual void OnAcceptButtonClickedInternal();
	UFUNCTION()
		virtual void OnCancelButtonClickedInternal();

private:
	FScriptDelegate OnAcceptButtonClicked;
	FScriptDelegate OnCancelButtonClicked;

public:
	UPROPERTY(EditAnywhere, Category = "Events")
		FDialogWindowEvent OnShowDialog;
	UPROPERTY(EditAnywhere, Category = "Events")
		FDialogWindowResponseEvent OnDialogCompleted;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UHexUIButton* LeftButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UHexUIButton* RightButton;

private:
	UTexture2D* acceptButonIconTexture;
	UTexture2D* okButonIconTexture;
	UTexture2D* yesButonIconTexture;
	UTexture2D* cancelButonIconTexture;
	UTexture2D* noButonIconTexture;

	FDialogButtonsButtons buttonsEnum;
	FDialogWindowResponse responseEnum;

	void OnCloseButtonClicked_Delayed();
	void OnCloseButtonClicked_Callback();
};
