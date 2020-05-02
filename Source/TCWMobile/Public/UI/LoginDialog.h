// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "PopupWindow.h"

#include "LoginDialog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoginDialogSignature);

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
	void DisplaySelfEvent(UWidget* contentWidget) override;

	UFUNCTION()
		virtual void DisplayLoginEvent();
	UFUNCTION()
		virtual void DisplayRegisterEvent();
	UFUNCTION()
		void EmailTextChangedEvent(const FText& text);
	UFUNCTION()
		void PasswordTextChangedEvent(const FText& text);

	virtual void OnAcceptButtonClickedInternal();

private:
	FScriptDelegate OnAcceptButtonClicked;

	bool bIsRegistering;

	void UserLoginInternal();
	void UserRegistrationInternal();

	void LoginFailureEvent();
	void RegisterFailureEvent();

public:
	UPROPERTY(BlueprintCallable, Category = "Events")
		FLoginDialogSignature OnDisplayLogin;
	UPROPERTY(BlueprintCallable, Category = "Events")
		FLoginDialogSignature OnDisplayRegister;

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
