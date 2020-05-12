// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "PopupWindow.h"
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
		ContentBorder->AddChild(contentWidget);

	PlayAnimation(DisplaySelf);
}

void UPopupWindow::OnCloseButtonClickedInternal()
{
	UKismetSystemLibrary::Delay(this, 0.3f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.3f);

	PlayAnimation(DisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f, false);

	UKismetSystemLibrary::Delay(this, 0.3f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.3f);

	RemoveFromParent();

	this->Destruct();
}
