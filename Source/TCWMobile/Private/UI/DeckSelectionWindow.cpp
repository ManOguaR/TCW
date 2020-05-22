// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DeckSelectionWindow.h"
#include "HexUIButton.h"

UDeckSelectionWindow::UDeckSelectionWindow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnDisplayWindow.AddDynamic(this, &UDeckSelectionWindow::DisplayWindow);
	OnCloseWindowClicked.BindUFunction(this, "OnCloseButtonClickedInternal");
}

void UDeckSelectionWindow::NativeDestruct()
{

}

void UDeckSelectionWindow::DisplayWindow()
{
	CloseWindowButton->OnClicked.AddUnique(OnCloseWindowClicked);
	PlayAnimation(DisplaySelf);
}

void UDeckSelectionWindow::OnCloseButtonClickedInternal()
{
	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UDeckSelectionWindow::OnCloseButtonClicked_Delayed, 0.3f);
}
void UDeckSelectionWindow::OnCloseButtonClicked_Delayed()
{
	PlayAnimation(DisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f, false);

	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UDeckSelectionWindow::OnCloseButtonClicked_Callback, 0.3f);
}
void UDeckSelectionWindow::OnCloseButtonClicked_Callback()
{
	RemoveFromParent();
	OnWindowClosed.ExecuteIfBound();
	this->Destruct();
}