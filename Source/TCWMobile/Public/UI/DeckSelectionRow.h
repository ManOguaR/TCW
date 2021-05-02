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
	UDeckSelectionRow(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FDeckSelectionRowEvent OnClicked;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Deck_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* DeckNameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBorder* RowImage_Border;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* ShadowImage;

	UFUNCTION(BlueprintCallable)
		void SetupRow(UWidget* widgetParent, FText rowName, UImage* rowImage);

	UFUNCTION(BlueprintCallable)
		void ToggleSelection(bool forceSelected = false);

private:
	FString CardSetName;
	UWidget* WidgetParent;

	bool bIsSelected;

	UFUNCTION()
		void OnDeckButtonClicked();
};
