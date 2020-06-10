// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "DeckSelectionRow.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SlateCore.h"

UDeckSelectionRow::UDeckSelectionRow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

void UDeckSelectionRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	Deck_Button->OnClicked.AddDynamic(this, &UDeckSelectionRow::OnDeckButtonClicked);
}

void UDeckSelectionRow::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDeckSelectionRow::SetupRow(UWidget* widgetParent, FText rowName, UImage* rowImage)
{
	WidgetParent = widgetParent;
	CardSetName = rowName.ToString();
	DeckNameText->SetText(rowName);

	if (rowImage->IsValidLowLevel())
	{
		RowImage_Border->SetBrush(FSlateImageBrush(rowImage, FVector2D(32.0f), FSlateColor()));
	}
}

void UDeckSelectionRow::OnDeckButtonClicked()
{
	if (OnClicked.IsBound())
		OnClicked.Broadcast(CardSetName, this);
}