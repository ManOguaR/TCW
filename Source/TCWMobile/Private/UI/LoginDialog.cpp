// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "LoginDialog.h"
#include "SystemFunctionLibrary.h"
#include "ValidityFunctionLibrary.h"

ULoginDialog::ULoginDialog(const FObjectInitializer& ObjectInitializer) :UPopupWindow(ObjectInitializer)
{
	//PRE - 14
	OnDisplayLogin.AddDynamic(this, &ULoginDialog::DisplayLoginEvent);
	OnDisplayRegister.AddDynamic(this, &ULoginDialog::DisplayRegisterEvent);

	OnAcceptButtonClicked.BindUFunction(this, "OnAcceptButtonClickedInternal");
}

void ULoginDialog::NativeConstruct()
{
	Register_Email->OnTextChanged.AddDynamic(this, &ULoginDialog::EmailTextChangedEvent);
	Register_Password->OnTextChanged.AddDynamic(this, &ULoginDialog::PasswordTextChangedEvent);
}

void ULoginDialog::NativeDestruct()
{
	AcceptDialogButton->OnClicked.Remove(OnAcceptButtonClicked);

	UPlayFabManager* accountManager = USystemFunctionLibrary::GetTCWGameInstance(this)->GetAccountManager();
	accountManager->OnOperationSuccess.Remove(this, "LoginSuccessEvent");
	accountManager->OnOperationFailure.Remove(this, "LoginFailureEvent");

	UPopupWindow::NativeDestruct();
}

void ULoginDialog::DisplaySelfEvent(UWidget* contentWidget)
{

}

void ULoginDialog::DisplayLoginEvent()
{
	bIsRegistering = false;
	LoginRegister_Switcher->ActiveWidgetIndex = 0;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Login_Username->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;

	UPopupWindow::DisplaySelfEvent(nullptr);
}

void ULoginDialog::DisplayRegisterEvent()
{
	bIsRegistering = true;
	LoginRegister_Switcher->ActiveWidgetIndex = 1;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Register_Username->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;

	AcceptDialogButton->OnClicked.AddUnique(OnAcceptButtonClicked);

	UPopupWindow::DisplaySelfEvent(nullptr);
}

void ULoginDialog::OnAcceptButtonClickedInternal()
{
	if (bIsRegistering)
		UserRegistrationInternal();
	else
		UserLoginInternal();
}

void ULoginDialog::UserLoginInternal()
{
	UPlayFabManager* accountManager = USystemFunctionLibrary::GetTCWGameInstance(this)->GetAccountManager();
	accountManager->OnOperationSuccess.AddUniqueDynamic(this, &ULoginDialog::OnCloseButtonClickedInternal);
	accountManager->OnOperationFailure.AddUniqueDynamic(this, &ULoginDialog::LoginFailureEvent);
	accountManager->UsernameLogin(Login_Username->GetText().ToString(), Login_Password->GetText().ToString());
}

void ULoginDialog::UserRegistrationInternal()
{
	UPlayFabManager* accountManager = USystemFunctionLibrary::GetTCWGameInstance(this)->GetAccountManager();
	accountManager->OnOperationSuccess.AddUniqueDynamic(this, &ULoginDialog::OnCloseButtonClickedInternal);
	accountManager->OnOperationFailure.AddUniqueDynamic(this, &ULoginDialog::RegisterFailureEvent);
	accountManager->CreateAccount(Register_Username->GetText().ToString(), Register_Email->GetText().ToString(), Register_Password->GetText().ToString());
}

void ULoginDialog::EmailTextChangedEvent(const FText& text)
{
	if (UValidityFunctionLibrary::IsEmailValid(text.ToString()))
	{
		RegisterErrorMessage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RegisterErrorMessage->SetText(FText::FromString(TEXT("Invalid Email")));
		RegisterErrorMessage->SetVisibility(ESlateVisibility::Visible);
	}
}
void ULoginDialog::PasswordTextChangedEvent(const FText& text)
{
	if (UValidityFunctionLibrary::IsPasswordValid(text.ToString(), FPasswordValidationRules(8, 16, true, false, true, true)))
	{
		RegisterErrorMessage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RegisterErrorMessage->SetText(FText::FromString(TEXT("Invalid password.")));
		RegisterErrorMessage->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginDialog::LoginFailureEvent()
{
	RegisterErrorMessage->SetText(FText::FromString(TEXT("Invalid username or password")));
	RegisterErrorMessage->SetVisibility(ESlateVisibility::Visible);
}

void ULoginDialog::RegisterFailureEvent()
{
	RegisterErrorMessage->SetText(FText::FromString(TEXT("Something went wrong...")));
	RegisterErrorMessage->SetVisibility(ESlateVisibility::Visible);
}

