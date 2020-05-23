// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "PopupWindow.h"

#include "LoginDialog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoginDialogEvent);

/**
*
*/
UCLASS()
class TCWMOBILE_API ULoginDialog : public UPopupWindow
{
	GENERATED_BODY()

public:
	ULoginDialog(const FObjectInitializer& ObjectInitializer);

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void DisplayPopupWindow(UWidget* contentWidget) override;

	UFUNCTION()
		virtual void DisplayLogin();
	UFUNCTION()
		virtual void DisplayRegister();
	UFUNCTION()
		void EmailTextChanged(const FText& text);
	UFUNCTION()
		void PasswordTextChanged(const FText& text);

	virtual void OnAcceptButtonClickedInternal();

private:
	FScriptDelegate OnAcceptButtonClicked;

	bool bIsRegistering;

	void UserLoginInternal();
	void UserRegistrationInternal();

	void LoginFailure();
	void RegisterFailure();

public:
	UPROPERTY(BlueprintCallable, Category = "Events")
		FLoginDialogEvent OnDisplayLogin;
	UPROPERTY(BlueprintCallable, Category = "Events")
		FLoginDialogEvent OnDisplayRegister;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UHexUIButton* AcceptDialogButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* LoginRegister_Switcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* Login_Password;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* Login_Username;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* Register_Password;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* Register_Username;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* Register_Email;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* LoginErrorMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* RegisterErrorMessage;
};
