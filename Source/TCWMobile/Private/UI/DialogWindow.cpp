// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DialogWindow.h"
#include "UObject\ConstructorHelpers.h"
#include "MiscFunctionLibrary.h"
#include "Components\TextBlock.h"
#include "Blueprint\WidgetTree.h"
#include "SlateGlobals.h"
#include "Components/Widget.h"
#include "Kismet/KismetSystemLibrary.h"

UDialogWindow::UDialogWindow(const FObjectInitializer& ObjectInitializer) :
	UPopupWindow(ObjectInitializer)
{
	//PRE - 08
	acceptButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'")).Object;
	okButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'")).Object;
	yesButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'")).Object;

	cancelButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_CrossIcon.TCW_CrossIcon'")).Object;
	noButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_CrossIcon.TCW_CrossIcon'")).Object;

	OnAcceptButtonClicked.BindUFunction(this, "OnAcceptButtonClickedInternal");
	OnCancelButtonClicked.BindUFunction(this, "OnCancelButtonClickedInternal");
}

void UDialogWindow::NativeDestruct()
{
	LeftButton->OnClicked.Remove(OnAcceptButtonClicked);
	RightButton->OnClicked.Remove(OnCancelButtonClicked);
	UPopupWindow::NativeDestruct();
}

FDialogWindowResponse UDialogWindow::GetResponse()
{
	if (responseEnum != FDialogWindowResponse::DialogWindow_Unknown)
	{
		const FDialogWindowResponse dialogResponse = responseEnum;
		this->Destruct();
		return dialogResponse;
	}
	return responseEnum;
}

void UDialogWindow::ShowDialogEvent(FDialogButtonsButtons buttons, FString message)
{
	buttonsEnum = buttons;
	responseEnum = FDialogWindowResponse::DialogWindow_Unknown;

	switch (buttonsEnum)
	{
	case FDialogButtonsButtons::DialogButtons_OK:
		RightButton->IconTexture = okButonIconTexture;
		RightButton->OnClicked.AddUnique(OnAcceptButtonClicked);
		RightButton->SetToolTipText(FText::FromString(TEXT("Ok")));
		LeftButton->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case FDialogButtonsButtons::DialogButtons_OKCancel:
		LeftButton->IconTexture = okButonIconTexture;
		LeftButton->OnClicked.AddUnique(OnAcceptButtonClicked);
		LeftButton->SetToolTipText(FText::FromString(TEXT("Ok")));
		RightButton->IconTexture = cancelButonIconTexture;
		RightButton->OnClicked.AddUnique(OnCancelButtonClicked);
		RightButton->SetToolTipText(FText::FromString(TEXT("Cancel")));
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case FDialogButtonsButtons::DialogButtons_Accept:
		RightButton->IconTexture = acceptButonIconTexture;
		RightButton->OnClicked.AddUnique(OnAcceptButtonClicked);
		RightButton->SetToolTipText(FText::FromString(TEXT("Accept")));
		LeftButton->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case FDialogButtonsButtons::DialogButtons_AcceptCancel:
		LeftButton->IconTexture = acceptButonIconTexture;
		LeftButton->OnClicked.AddUnique(OnAcceptButtonClicked);
		LeftButton->SetToolTipText(FText::FromString(TEXT("Accept")));
		RightButton->IconTexture = cancelButonIconTexture;
		RightButton->OnClicked.AddUnique(OnCancelButtonClicked);
		RightButton->SetToolTipText(FText::FromString(TEXT("Cancel")));
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case FDialogButtonsButtons::DialogButtons_YesNo:
		LeftButton->IconTexture = yesButonIconTexture;
		LeftButton->OnClicked.AddUnique(OnAcceptButtonClicked);
		LeftButton->SetToolTipText(FText::FromString(TEXT("Yes")));
		RightButton->IconTexture = noButonIconTexture;
		RightButton->OnClicked.AddUnique(OnCancelButtonClicked);
		RightButton->SetToolTipText(FText::FromString(TEXT("No")));
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case FDialogButtonsButtons::DialogButtons_Cancel:
		RightButton->IconTexture = cancelButonIconTexture;
		RightButton->OnClicked.AddUnique(OnCancelButtonClicked);
		RightButton->SetToolTipText(FText::FromString(TEXT("Cancel")));
		LeftButton->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}

	UTextBlock* content = WidgetTree->ConstructWidget<UTextBlock>();// CreateWidget<UTextBlock>(this);
	content->SetText(FText::FromString(message));

	UPopupWindow::DisplaySelfEvent(content);
}

void UDialogWindow::OnCloseButtonClickedInternal()
{
	UKismetSystemLibrary::Delay(this, 0.3f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.3f);

	PlayAnimation(DisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f, false);

	UKismetSystemLibrary::Delay(this, 0.3f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.3f);

	RemoveFromParent();
}

void UDialogWindow::OnAcceptButtonClickedInternal()
{
	responseEnum = ((buttonsEnum == FDialogButtonsButtons::DialogButtons_Accept || buttonsEnum == FDialogButtonsButtons::DialogButtons_AcceptCancel) ? FDialogWindowResponse::DialogWindow_Accept :
		((buttonsEnum == FDialogButtonsButtons::DialogButtons_OK || buttonsEnum == FDialogButtonsButtons::DialogButtons_OKCancel) ? FDialogWindowResponse::DialogWindow_OK :
			FDialogWindowResponse::DialogWindow_Yes));

	OnCloseButtonClickedInternal();
}

void UDialogWindow::OnCancelButtonClickedInternal()
{
	responseEnum = ((buttonsEnum == FDialogButtonsButtons::DialogButtons_AcceptCancel || buttonsEnum == FDialogButtonsButtons::DialogButtons_Cancel || buttonsEnum == FDialogButtonsButtons::DialogButtons_OKCancel) ? FDialogWindowResponse::DialogWindow_Cancel :
		FDialogWindowResponse::DialogWindow_No);

	OnCloseButtonClickedInternal();
}
