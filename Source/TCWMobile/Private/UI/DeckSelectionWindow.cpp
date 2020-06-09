// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DeckSelectionWindow.h"
#include "Account\TCWSaveGame.h"
#include "HexUIButton.h"
#include "SaveGameFunctionLibrary.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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

	PopulateDeckList();

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

void UDeckSelectionWindow::PopulateDeckList()
{
	bool isSuccess;
	USaveGame* saveGame = USaveGameFunctionLibrary::CreateLoadCardGameSave("CardGameSave", UTCWSaveGame::StaticClass(), isSuccess);

	if (isSuccess)
	{
		if (UTCWSaveGame* castSave = Cast<UTCWSaveGame>(saveGame))
		{
			if (castSave->DeckList.Num() > 0)
			{
				for (FString deckName : castSave->DeckList)
				{
					if (UGameplayStatics::DoesSaveGameExist(deckName, 0))
					{

					}
				}
			}
		}
	}
}