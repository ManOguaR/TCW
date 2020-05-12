// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"

#include "Enums.h"
#include "UIEnums.h"

#include "CardManager.generated.h"

class ATCWPlayerState;
class ATCWPlayerController;
class UCardWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCardManagerEvent);

/**
 *
 */
UCLASS()
class TCWMOBILE_API UCardManager : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Events")
		FCardManagerEvent OnUpdateCardPositions;
	UPROPERTY(EditAnywhere, Category = "Events")
		FCardManagerEvent OnToggleDisplayCardEditor;

	UPROPERTY(EditAnywhere, Category = "System")
		EViewState DraggableCardViewState;

public:
	UCardManager(const FObjectInitializer& ObjectInitializer);

	// Cards
	TArray<UCardWidget*> ActiveCardsInHand;
	//System Enums
	EViewState CardViewState;

protected:
	// Cards
	int32 CardsInHand;
	float CardScale;
	bool bRotateToOffset;

	//System Enums
	EPositioningProfile PositioningProfile;

	//System
	float ViewportScale;
	bool LockOnEditorChangeState;
	FVector2D ScreenRes;
	TArray<FWidgetTransform> GoalLocationArray;
	ATCWPlayerState* OwnerPlayerState;
	ATCWPlayerController* OwnerPlayerController;
	UCardWidget* TempCardWidget;
	bool bDarkenBackground;

	//Card Editor
	ECardSet EditorCardSet;
	FName EditorCardName;
	TArray<FString> EditorCardSetList;

	//Card Layout Profile
	int32 MaxCards;
	float RadiusOffset;
	float CardSpacing;
	float ScreenOffsetX;
	float ScreenOffsetY;
	float CardOffset;
	float RotationDistanceScaleFactor;
	float DynamicSpacingMaxOffset;
	FVector2D CardSize;
	FVector2D TransformPivot;
	FVector2D GoalTransform;
	bool bFlyInDirection;
	bool bIsFaceUp;
	bool bDynamicSpacing;

private:
	bool bEditorDisplayed;
	bool bIsInit;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* CardEditorTransition;			//IN USE

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* CanvasPanel_0;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* BackgroundDarken_Image;					//IN USE-EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Card_Panel;						//IN USE

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Card_Editor_Panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Fade_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Background;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* EditorBackground_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* EditorBackground_Image2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* EditorBackground_Image3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* EditorFade_Image_4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* PositioningFeatures_Button;			//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* CardsAndDeck_Button;					//EVENT

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* WidgetSwitcher_42;				//IN USE - RENAME
	/* Positioning features panel */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_245;													//TODO
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* RotateCards_CheckBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* LeftToRight_CheckBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_511;													//TODO
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* PositioningProfile_ComboBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ResetProfile_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* LayoutProfile_ComboBox;		//IN USE
	/* Card editor panel */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ResetDeck_Button;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ClearHand_Button;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* AddCard_Button;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* CardList_ComboBox;				//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CardTools_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* CardSet_ComboBox;				//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* PickupCard_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* SkipCardCost_CheckBox;

	/* Location offset panel */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Background_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* XOffset_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* YOffset_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* SliderBackground_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* OffsetX_Slider;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* OffsetX_TextBox;				//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* OffsetY_TextBox;				//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* SliderBackground_Image2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* OffsetY_Slider;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* LocationOffsetHeader_Image;
	/* Editable values panel */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Background_Image2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* EditableValuesHeader_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* RotationDistanceOffset_TextBox;	//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* CardSpacing_TextBox;			//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* SliderBackgroundImage_CardGap;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* PosOffset_Slider;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* SliderBackgroundImage_Radius;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* Radius_Slider;							//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* RotationDistanceOffset_RotDist;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* RotationDistanceOffset_Slider;			//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* Radius_TextBox;				//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* RotationDistanceOffset_Scale;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* CardScale_Slider;						//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* CardScale_TextBox;			//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CardOffsetSliderBg_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* CardOffset_TextBox;			//EVENT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* CardOffset_Slider;						//EVENT

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ToggleGameUI_Button;					//EVENT

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Editor_Button;							//EVENT

public:
	EViewState SetCardViewState(EViewState viewState, bool force);

protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;

private:
	UFUNCTION()
		void UpdateCardPositions();
	UFUNCTION()
		void ToggleDisplayCardEditor();

private:
	/* Card Functions */
	UCardWidget* AddCardWidget(FName cardName, ECardSet fromCardSet);
	FText GetCardoffsetValue();
	int32 RemoveCardWidgetFromHand(UCardWidget* cardToRemove, int32 orIndex, bool allCardsInHand);
	bool GetNumberOfCardsInHand(int32& outCardsInHand);
	FWidgetTransform GetCardPositionAtIndex(int32 index, UCardWidget* cardReference);
	void HighlightPlayableCardsInHand();

	/* Position Profile */
	FWidgetTransform CalculateHorizontalProfile(int32 cardIndex, UCardWidget* cardReference, int32 cardsInHand);
	FWidgetTransform CalculateFannedProfile(int32 cardIndex, UCardWidget* cardReference, int32 cardsInHand);
	void SetLayout(FName rowName);

	void DarkenBackground(bool enable);

private:
	/*INNER EVENTS*/
	UFUNCTION()
		void PositioningFeaturesButtonClicked();
	UFUNCTION()
		void CardsAndDeckButtonClicked();

	UFUNCTION()
		float RadiusSliderValueBound();
	UFUNCTION()
		void RadiusSliderOnValueChanged(float value);
	UFUNCTION()
		float PosOffsetSliderValueBound();
	UFUNCTION()
		void PosOffsetSliderOnValueChanged(float value);
	UFUNCTION()
		float RotationDistanceOffsetSliderValueBound();
	UFUNCTION()
		void RotationDistanceOffsetSliderOnValueChanged(float value);
	UFUNCTION()
		float CardScaleSliderValueBound();
	UFUNCTION()
		void CardScaleSliderOnValueChanged(float value);
	UFUNCTION()
		float CardOffsetSliderValueBound();
	UFUNCTION()
		void CardOffsetSliderOnValueChanged(float value);

	UFUNCTION()
		FText RadiusTextBoxValueBound();
	UFUNCTION()
		void RadiusTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);
	UFUNCTION()
		FText CardSpacingTextBoxValueBound();
	UFUNCTION()
		void CardSpacingTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);
	UFUNCTION()
		FText RotationDistanceOffsetTextBoxValueBound();
	UFUNCTION()
		void RotationDistanceOffsetTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);
	UFUNCTION()
		FText CardScaleTextBoxValueBound();
	UFUNCTION()
		void CardScaleTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);
	UFUNCTION()
		FText CardOffsetTextBoxValueBound();
	UFUNCTION()
		void CardOffsetTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);

	UFUNCTION()
		float OffsetXSliderValueBound();
	UFUNCTION()
		void OffsetXSliderOnValueChanged(float value);
	UFUNCTION()
		float OffsetYSliderValueBound();
	UFUNCTION()
		void OffsetYSliderOnValueChanged(float value);

	UFUNCTION()
		FText OffsetXTextBoxValueBound();
	UFUNCTION()
		void OffsetXTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);
	UFUNCTION()
		FText OffsetYTextBoxValueBound();
	UFUNCTION()
		void OffsetYTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod);

	UFUNCTION()
		ECheckBoxState RotateCardsCheckBoxValueBound();
	UFUNCTION()
		void RotateCardsCheckBoxOnCheckedStateChanged(bool bIsChecked);
	UFUNCTION()
		ECheckBoxState LeftToRightCheckBoxValueBound();
	UFUNCTION()
		void LeftToRightCheckBoxOnCheckedStateChanged(bool bIsChecked);

	UFUNCTION()
		UWidget* PositioningProfile_ComboBox_OnGenerateWidget(FString item);
	UFUNCTION()
		void PositioningProfile_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType);

	UFUNCTION()
		UWidget* LayoutProfile_ComboBox_OnGenerateWidget(FString item);
	UFUNCTION()
		void LayoutProfile_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType);

	UFUNCTION()
		void ResetProfile_ButtonClicked();

	UFUNCTION()
		void ResetDeck_ButtonClicked();
	UFUNCTION()
		void ClearHand_ButtonClicked();
	
	UFUNCTION()
		void AddCard_ButtonClicked();
	
	UFUNCTION()
		UWidget* CardList_ComboBox_OnGenerateWidget(FString item);
	UFUNCTION()
		void CardList_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType);

	UFUNCTION()
		UWidget* CardSet_ComboBox_OnGenerateWidget(FString item);
	UFUNCTION()
		void CardSet_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType);

	UFUNCTION()
		void PickupCard_ButtonClicked();
	
	UFUNCTION()
		ECheckBoxState SkipCardCost_CheckBoxValueBound();
	UFUNCTION()
		void SkipCardCost_CheckBoxOnCheckedStateChanged(bool bIsChecked);

	UFUNCTION()
		void ToggleGameUIButtonClicked();

	UFUNCTION()
		void EditorButtonClicked();

	UFUNCTION()
		FEventReply On_BackgroundDarken_Image_MouseButtonDown(FGeometry InGeometry, const FPointerEvent& InMouseEvent);
};
