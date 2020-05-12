// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DialogWindow.h"
#include "UObject/ConstructorHelpers.h"
#include "MiscFunctionLibrary.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Texture2D.h"
#include "SlateGlobals.h"
#include "Components/Widget.h"
#include "Kismet/KismetSystemLibrary.h"

// Define the namespace to use with LOCTEXT
// This is only valid within a single file, and must be undefined before the end of the file
#define LOCTEXT_NAMESPACE "DialogWindow"
// Create text literals
const FText TextAccept = LOCTEXT("DialogWindow_Accept", "Accept");
const FText TextOk = LOCTEXT("DialogWindow_Ok", "Ok");
const FText TextYes = LOCTEXT("DialogWindow_Yes", "Yes");
const FText TextCancel = LOCTEXT("DialogWindow_Cancel", "Cancel");
const FText TextNo = LOCTEXT("DialogWindow_No", "No");

UDialogWindow::UDialogWindow(const FObjectInitializer& ObjectInitializer) :
	UPopupWindow(ObjectInitializer)
{
	//PRE - 08
	ConstructorHelpers::FObjectFinder<UTexture2D> tickIcon(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'"));
	if (tickIcon.Succeeded())
	{
		acceptButonIconTexture = tickIcon.Object;
		okButonIconTexture = tickIcon.Object;
		yesButonIconTexture = tickIcon.Object;
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> crossIcon(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_CrossIcon.TCW_CrossIcon'"));
	if (tickIcon.Succeeded())
	{
		cancelButonIconTexture = crossIcon.Object;
		noButonIconTexture = crossIcon.Object;
	}

	//acceptButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'")).Object;
	//okButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'")).Object;
	//yesButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_TickIcon.TCW_TickIcon'")).Object;

	//cancelButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_CrossIcon.TCW_CrossIcon'")).Object;
	//noButonIconTexture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Textures/UserInterface/TCW_CrossIcon.TCW_CrossIcon'")).Object;
}

void UDialogWindow::NativeDestruct()
{
	LeftButton->OnClicked.RemoveAll(this);
	RightButton->OnClicked.RemoveAll(this);
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

void UDialogWindow::ShowDialog(FDialogButtonsButtons buttons, FString message)
{
	buttonsEnum = buttons;
	responseEnum = FDialogWindowResponse::DialogWindow_Unknown;

	switch (buttonsEnum)
	{
	case FDialogButtonsButtons::DialogButtons_OK:
		RightButton->IconTexture = okButonIconTexture;
		RightButton->OnClicked.AddDynamic(this, &UDialogWindow::OnAcceptButtonClickedInternal);
		RightButton->SetToolTipText(TextOk);
		LeftButton->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case FDialogButtonsButtons::DialogButtons_OKCancel:
		LeftButton->IconTexture = okButonIconTexture;
		LeftButton->OnClicked.AddDynamic(this, &UDialogWindow::OnAcceptButtonClickedInternal);
		LeftButton->SetToolTipText(TextOk);
		RightButton->IconTexture = cancelButonIconTexture;
		RightButton->OnClicked.AddDynamic(this, &UDialogWindow::OnCancelButtonClickedInternal);
		RightButton->SetToolTipText(TextCancel);
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case FDialogButtonsButtons::DialogButtons_Accept:
		RightButton->IconTexture = acceptButonIconTexture;
		RightButton->OnClicked.AddDynamic(this, &UDialogWindow::OnAcceptButtonClickedInternal);
		RightButton->SetToolTipText(TextAccept);
		LeftButton->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case FDialogButtonsButtons::DialogButtons_AcceptCancel:
		LeftButton->IconTexture = acceptButonIconTexture;
		LeftButton->OnClicked.AddDynamic(this, &UDialogWindow::OnAcceptButtonClickedInternal);
		LeftButton->SetToolTipText(TextAccept);
		RightButton->IconTexture = cancelButonIconTexture;
		RightButton->OnClicked.AddDynamic(this, &UDialogWindow::OnCancelButtonClickedInternal);
		RightButton->SetToolTipText(TextCancel);
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case FDialogButtonsButtons::DialogButtons_YesNo:
		LeftButton->IconTexture = yesButonIconTexture;
		LeftButton->OnClicked.AddDynamic(this, &UDialogWindow::OnAcceptButtonClickedInternal);
		LeftButton->SetToolTipText(TextYes);
		RightButton->IconTexture = noButonIconTexture;
		RightButton->OnClicked.AddDynamic(this, &UDialogWindow::OnCancelButtonClickedInternal);
		RightButton->SetToolTipText(TextNo);
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case FDialogButtonsButtons::DialogButtons_Cancel:
		RightButton->IconTexture = cancelButonIconTexture;
		RightButton->OnClicked.AddDynamic(this, &UDialogWindow::OnCancelButtonClickedInternal);
		RightButton->SetToolTipText(TextCancel);
		LeftButton->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}

	UTextBlock* content = WidgetTree->ConstructWidget<UTextBlock>();// CreateWidget<UTextBlock>(this);
	content->SetText(FText::FromString(message));

	Super::DisplayPopupWindow(content);
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

// Undefine the namespace before the end of the file
#undef LOCTEXT_NAMESPACE
