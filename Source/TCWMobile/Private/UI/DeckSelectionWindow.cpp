// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DeckSelectionWindow.h"
#include "Account\TCWSaveGame.h"
#include "DeckSelectionRow.h"
#include "HexUIButton.h"
#include "SaveGameFunctionLibrary.h"
#include "SystemFunctionLibrary.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/UniformGridPanel.h"

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
	EditDeck_Button->OnClicked.AddDynamic(this, &UDeckSelectionWindow::EditDeckClicked);
	SelectDeck_Button->OnClicked.AddDynamic(this, &UDeckSelectionWindow::PlayClicked);
	NewDeck_Button->OnClicked.AddDynamic(this, &UDeckSelectionWindow::NewDeckClicked);

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

	int32 row = 0, column = 0;
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
						if (column == 0 && row == 0)
							SelectedDeck = deckName;

						column = column + 1;
						if (column > 2)
						{
							column = 0;
							row = row + 1;
						}

						//WidgetBlueprint'/Game/Blueprints/Widgets/MainMenu/DeckSelectionRowWidget.DeckSelectionRowWidget'
						FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/DeckSelectionRowWidget.DeckSelectionRowWidget_C"));
						if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UDeckSelectionRow>())
						{
							UDeckSelectionRow* deckRow = CreateWidget<UDeckSelectionRow>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);

							deckRow->SetupRow(this, FText::FromString(deckName), nullptr);
							deckRow->OnClicked.AddDynamic(this, &UDeckSelectionWindow::SelectDeck);

							DeckSelection_GridPanel->AddChildToUniformGrid(deckRow, row, column);

							if (SelectedDeck == deckName)
							{
								deckRow->ToggleSelection(true);
							}
						}
					}
				}
			}
		}
	}
}

void UDeckSelectionWindow::SelectDeck(FString cardSetName, UDeckSelectionRow* callerWidget)
{
	callerWidget->ToggleSelection(SelectedDeck == cardSetName);

	SelectedDeck = cardSetName;
}

void UDeckSelectionWindow::NewDeckClicked()
{
	RemoveFromParent();
	OnWindowClosed.ExecuteIfBound();
	USystemFunctionLibrary::GetTCWGameInstance(this)->OnLoadCollectionManager.Broadcast("NEW");
	this->Destruct();
}

void UDeckSelectionWindow::EditDeckClicked()
{
	RemoveFromParent();
	OnWindowClosed.ExecuteIfBound();
	USystemFunctionLibrary::GetTCWGameInstance(this)->OnLoadCollectionManager.Broadcast(SelectedDeck);
	this->Destruct();
}

void UDeckSelectionWindow::PlayClicked()
{
	RemoveFromParent();
	OnWindowClosed.ExecuteIfBound();
	USystemFunctionLibrary::GetTCWGameInstance(this)->OnHostGame.Broadcast(SelectedDeck);
	this->Destruct();
}
