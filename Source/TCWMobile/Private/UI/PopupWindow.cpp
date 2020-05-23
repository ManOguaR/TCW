// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "PopupWindow.h"
#include "HexUIButton.h"
#include "MiscFunctionLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

UPopupWindow::UPopupWindow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	//PRE - 07
	OnDisplayPopupWindow.AddDynamic(this, &UPopupWindow::DisplayPopupWindow);
	OnCloseWindowClicked.BindUFunction(this, "OnCloseButtonClickedInternal");
}

void UPopupWindow::NativeDestruct()
{
	ContentBorder->ClearChildren();
	CloseWindowButton->OnClicked.Remove(OnCloseWindowClicked);
}

void UPopupWindow::DisplayPopupWindow(UWidget* contentWidget)
{
	CloseWindowButton->OnClicked.AddUnique(OnCloseWindowClicked);

	if (contentWidget->IsValidLowLevel())
	{
		//contentWidget->Register();
		ContentBorder->AddChild(contentWidget);
		//this->UpdateRenderTransform();
	}
	PlayAnimation(DisplaySelf);
}

void UPopupWindow::OnCloseButtonClickedInternal()
{
	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UPopupWindow::OnCloseButtonClicked_Delayed, 0.3f);
}
void UPopupWindow::OnCloseButtonClicked_Delayed()
{
	PlayAnimation(DisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f, false);

	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UPopupWindow::OnCloseButtonClicked_Callback, 0.3f);
}
void UPopupWindow::OnCloseButtonClicked_Callback()
{
	RemoveFromParent();

	this->Destruct();
}