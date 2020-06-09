// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeckSelectionRow.generated.h"

class UBorder;
class UButton;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeckSelectionRowEvent, FString, cardSetName, UDeckSelectionRow*, callerWidget);
/**
 *
 */
UCLASS()
class TCWMOBILE_API UDeckSelectionRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FDeckSelectionRowEvent OnClicked;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Deck_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* HexUIIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBorder* RowImage_Border;

	//UFUNCTION(BlueprintCallable)
	//	void SetupRow(UWidget* widgetParent, FText rowName, UImage* rowImage, FDeckSelectionRowEvent delegate);

private:
	FString CardSetName;
	UWidget* WidgetParent;
};
