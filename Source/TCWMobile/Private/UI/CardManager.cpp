// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "CardManager.h"
#include "CardLayoutFunctionLibrary.h"
#include "CardWidget.h"
#include "ComboBoxOption.h"
#include "DeckFunctionLibrary.h"
#include "MiscFunctionLibrary.h"
#include "TCWPlayerController.h"
#include "UIStructs.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"

UCardManager::UCardManager(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnUpdateCardPositions.AddDynamic(this, &UCardManager::UpdateCardPositions);
	OnToggleDisplayCardEditor.AddDynamic(this, &UCardManager::ToggleDisplayCardEditor);
}

//Public Access Functions
EViewState UCardManager::SetCardViewState(EViewState viewState, bool force)
{
	if (force || viewState != CardViewState)
	{
		CardViewState = viewState;

		switch (CardViewState)
		{
		case EViewState::ViewState_Default:
		{
			DarkenBackground(false);
			SetLayout(PositioningProfile == EPositioningProfile::PositioningProfile_Fanned ? FName("Fanned_Left_Default") : FName("Horizontal_Default"));
			break;
		}
		case EViewState::ViewState_ViewHand:
		{
			DarkenBackground(true);
			SetLayout(FName("View_Hand_Default"));
			break;
		}
		case EViewState::ViewState_HideHand:
		{
			DarkenBackground(false);
			SetLayout(PositioningProfile == EPositioningProfile::PositioningProfile_Fanned ? FName("Hide_Hand_Fanned_Default") : FName("Horizontal_Hide_Hand_Default"));
			break;
		}
		case EViewState::ViewState_TestProfile:
		{
			DarkenBackground(false);
			SetLayout(FName("test"));
			break;
		}
		case EViewState::ViewState_CardInPreview:
		default:
			break;
		}
	}
	if (CardViewState != EViewState::ViewState_CardInPreview && OnUpdateCardPositions.IsBound())
		OnUpdateCardPositions.Broadcast();

	return CardViewState;
}

// OVERRIDES
void UCardManager::NativePreConstruct()
{
	//Bind Events On created Elements
	PositioningFeatures_Button->OnClicked.AddDynamic(this, &UCardManager::PositioningFeaturesButtonClicked);
	CardsAndDeck_Button->OnClicked.AddDynamic(this, &UCardManager::CardsAndDeckButtonClicked);

	Radius_Slider->ValueDelegate.BindDynamic(this, &UCardManager::RadiusSliderValueBound);
	Radius_Slider->OnValueChanged.AddDynamic(this, &UCardManager::RadiusSliderOnValueChanged);
	PosOffset_Slider->ValueDelegate.BindDynamic(this, &UCardManager::PosOffsetSliderValueBound);
	PosOffset_Slider->OnValueChanged.AddDynamic(this, &UCardManager::PosOffsetSliderOnValueChanged);
	RotationDistanceOffset_Slider->ValueDelegate.BindDynamic(this, &UCardManager::RotationDistanceOffsetSliderValueBound);
	RotationDistanceOffset_Slider->OnValueChanged.AddDynamic(this, &UCardManager::RotationDistanceOffsetSliderOnValueChanged);
	CardScale_Slider->ValueDelegate.BindDynamic(this, &UCardManager::CardScaleSliderValueBound);
	CardScale_Slider->OnValueChanged.AddDynamic(this, &UCardManager::CardScaleSliderOnValueChanged);
	//CardOffset_Slider->ValueDelegate.BindDynamic(this, &UCardManager::CardOffsetSliderValueBound);
	CardOffset_Slider->OnValueChanged.AddDynamic(this, &UCardManager::CardOffsetSliderOnValueChanged);

	Radius_TextBox->TextDelegate.BindDynamic(this, &UCardManager::RadiusTextBoxValueBound);
	Radius_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::RadiusTextBoxOnTextCommited);
	CardSpacing_TextBox->TextDelegate.BindDynamic(this, &UCardManager::CardSpacingTextBoxValueBound);
	CardSpacing_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::CardSpacingTextBoxOnTextCommited);
	RotationDistanceOffset_TextBox->TextDelegate.BindDynamic(this, &UCardManager::RotationDistanceOffsetTextBoxValueBound);
	//RotationDistanceOffset_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::RotationDistanceOffsetTextBoxOnTextCommited);
	CardScale_TextBox->TextDelegate.BindDynamic(this, &UCardManager::CardScaleTextBoxValueBound);
	//CardScale_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::CardScaleTextBoxOnTextCommited);
	CardOffset_TextBox->TextDelegate.BindDynamic(this, &UCardManager::CardOffsetTextBoxValueBound);
	//CardOffset_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::CardOffsetTextBoxOnTextCommited);

	OffsetX_Slider->ValueDelegate.BindDynamic(this, &UCardManager::OffsetXSliderValueBound);
	OffsetX_Slider->OnValueChanged.AddDynamic(this, &UCardManager::OffsetXSliderOnValueChanged);
	OffsetY_Slider->ValueDelegate.BindDynamic(this, &UCardManager::OffsetYSliderValueBound);
	OffsetY_Slider->OnValueChanged.AddDynamic(this, &UCardManager::OffsetYSliderOnValueChanged);

	OffsetX_TextBox->TextDelegate.BindDynamic(this, &UCardManager::OffsetXTextBoxValueBound);
	//OffsetX_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::OffsetXTextBoxOnTextCommited);
	OffsetY_TextBox->TextDelegate.BindDynamic(this, &UCardManager::OffsetYTextBoxValueBound);
	//OffsetY_TextBox->OnTextCommitted.AddDynamic(this, &UCardManager::OffsetYTextBoxOnTextCommited);

	RotateCards_CheckBox->CheckedStateDelegate.BindDynamic(this, &UCardManager::RotateCardsCheckBoxValueBound);
	RotateCards_CheckBox->OnCheckStateChanged.AddDynamic(this, &UCardManager::RotateCardsCheckBoxOnCheckedStateChanged);
	LeftToRight_CheckBox->CheckedStateDelegate.BindDynamic(this, &UCardManager::LeftToRightCheckBoxValueBound);
	LeftToRight_CheckBox->OnCheckStateChanged.AddDynamic(this, &UCardManager::LeftToRightCheckBoxOnCheckedStateChanged);

	PositioningProfile_ComboBox->OnGenerateWidgetEvent.BindDynamic(this, &UCardManager::PositioningProfile_ComboBox_OnGenerateWidget);
	PositioningProfile_ComboBox->OnSelectionChanged.AddDynamic(this, &UCardManager::PositioningProfile_ComboBox_OnSelectionChanged);

	LayoutProfile_ComboBox->OnGenerateWidgetEvent.BindDynamic(this, &UCardManager::LayoutProfile_ComboBox_OnGenerateWidget);
	LayoutProfile_ComboBox->OnSelectionChanged.AddDynamic(this, &UCardManager::LayoutProfile_ComboBox_OnSelectionChanged);

	ResetProfile_Button->OnClicked.AddDynamic(this, &UCardManager::ResetProfile_ButtonClicked);

	ResetDeck_Button->OnClicked.AddDynamic(this, &UCardManager::ResetDeck_ButtonClicked);
	ClearHand_Button->OnClicked.AddDynamic(this, &UCardManager::ClearHand_ButtonClicked);

	AddCard_Button->OnClicked.AddDynamic(this, &UCardManager::AddCard_ButtonClicked);

	CardList_ComboBox->OnGenerateWidgetEvent.BindDynamic(this, &UCardManager::CardList_ComboBox_OnGenerateWidget);
	CardList_ComboBox->OnSelectionChanged.AddDynamic(this, &UCardManager::CardList_ComboBox_OnSelectionChanged);

	CardSet_ComboBox->OnGenerateWidgetEvent.BindDynamic(this, &UCardManager::CardSet_ComboBox_OnGenerateWidget);
	CardSet_ComboBox->OnSelectionChanged.AddDynamic(this, &UCardManager::CardSet_ComboBox_OnSelectionChanged);

	PickupCard_Button->OnClicked.AddDynamic(this, &UCardManager::PickupCard_ButtonClicked);

	//SkipCardCost_CheckBox->CheckedStateDelegate.BindDynamic(this, &UCardManager::SkipCardCost_CheckBoxValueBound);
	SkipCardCost_CheckBox->OnCheckStateChanged.AddDynamic(this, &UCardManager::SkipCardCost_CheckBoxOnCheckedStateChanged);

	ToggleGameUI_Button->OnClicked.AddDynamic(this, &UCardManager::ToggleGameUIButtonClicked);

	Editor_Button->OnClicked.AddDynamic(this, &UCardManager::EditorButtonClicked);

	BackgroundDarken_Image->OnMouseButtonDownEvent.BindDynamic(this, &UCardManager::On_BackgroundDarken_Image_MouseButtonDown);
}

void UCardManager::NativeConstruct()
{
	ScreenRes = UWidgetLayoutLibrary::GetViewportSize(this);
	ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);

	if (ATCWPlayerController* pc = Cast<ATCWPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		OwnerPlayerController = pc;
		if (ATCWPlayerState* ps = Cast<ATCWPlayerState>(pc->PlayerState))
		{
			OwnerPlayerState = ps;
			SetCardViewState(EViewState::ViewState_Default, true);
		}
	}
}

//Event Implementations
void UCardManager::UpdateCardPositions()
{
	GoalLocationArray.Empty();
	int32 cardsInHand;
	if (GetNumberOfCardsInHand(cardsInHand))
	{
		int index = 0;
		for (UCardWidget* each : ActiveCardsInHand)
		{
			GoalLocationArray.EmplaceAt(index, GetCardPositionAtIndex(index, each));
			each->HomeTransform = GoalLocationArray[index];
			each->SetRenderTransformPivot(TransformPivot);
			each->OnOpenGate.Broadcast(each->HomeTransform, each, 10.0f, ECardWidgetMovementState::CardMovementState_Hand);
		}
	}
}

void UCardManager::ToggleDisplayCardEditor()
{
	bEditorDisplayed = !bEditorDisplayed;
	PlayAnimation(CardEditorTransition, 0.0f, 1, bEditorDisplayed ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	BackgroundDarken_Image->SetVisibility(bEditorDisplayed ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (!bIsInit)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> CardLayoutProfiles(TEXT("DataTable'/Game/Data/Gameplay/CardLayoutProfiles_DataTable.CardLayoutProfiles_DataTable'"));
		if (CardLayoutProfiles.Succeeded())
		{
			TArray<FName> rowNames;
			UDataTableFunctionLibrary::GetDataTableRowNames(CardLayoutProfiles.Object, rowNames);

			for (FName name : rowNames)
				LayoutProfile_ComboBox->AddOption(name.ToString());
		}

		bIsInit = true;
	}
}

/* Card Functions */
UCardWidget* UCardManager::AddCardWidget(FName cardName, ECardSet fromCardSet)
{
	TempCardWidget = CreateWidget<UCardWidget>(GetOwningPlayer());

	ActiveCardsInHand.Add(TempCardWidget);

	TempCardWidget->CardName = cardName;
	TempCardWidget->CardSetEnum = fromCardSet;
	TempCardWidget->bIsFaceUp = bIsFaceUp;
	TempCardWidget->UIParent = this;
	Card_Panel->AddChild(TempCardWidget);
	TempCardWidget->SetRenderTransformPivot(TransformPivot);

	UWidgetLayoutLibrary::SlotAsCanvasSlot(TempCardWidget)->SetLayout(UWidgetLayoutLibrary::SlotAsCanvasSlot(Card_Panel)->GetLayout());
	UWidgetLayoutLibrary::SlotAsCanvasSlot(TempCardWidget)->SetZOrder(1);
	TempCardWidget->SetRenderTransform(FWidgetTransform(FVector2D((((ScreenRes / 2.0f).X + 700) * (bFlyInDirection ? 1.0f : -1.0f)), 250.0f), FVector2D(1.0f), FVector2D(0.0f), bFlyInDirection ? 90.0f : -90.0f));

	if (bIsFaceUp)
	{
		TempCardWidget->SetCardData();
	}
	else
	{
		TempCardWidget->SetFaceDown(true);
	}
	OnUpdateCardPositions.Broadcast();
	HighlightPlayableCardsInHand();
	return TempCardWidget;
}

FText UCardManager::GetCardoffsetValue()
{
	return UKismetTextLibrary::Conv_FloatToText(CardOffset, ERoundingMode::FromZero);
}

int32 UCardManager::RemoveCardWidgetFromHand(UCardWidget* cardToRemove, int32 orIndex, bool allCardsInHand)
{
	int32 removedIndexLocal = 0;
	if (allCardsInHand)
	{
		for (UCardWidget* each : ActiveCardsInHand)
		{
			each->RemoveFromParent();
		}
		ActiveCardsInHand.Empty();
	}
	else
	{
		if (cardToRemove)
		{
			removedIndexLocal = ActiveCardsInHand.Find(cardToRemove);
			cardToRemove->RemoveFromParent();
			ActiveCardsInHand.Remove(cardToRemove);
		}
		else
		{
			removedIndexLocal = orIndex;
			ActiveCardsInHand[orIndex]->RemoveFromParent();
			ActiveCardsInHand.RemoveAt(orIndex);
		}
	}
	OnUpdateCardPositions.Broadcast();
	return removedIndexLocal;
}

bool UCardManager::GetNumberOfCardsInHand(int32& outCardsInHand)
{
	outCardsInHand = ActiveCardsInHand.Num();
	return outCardsInHand > 0;
}

FWidgetTransform UCardManager::GetCardPositionAtIndex(int32 index, UCardWidget* cardReference)
{
	return (PositioningProfile == EPositioningProfile::PositioningProfile_Fanned) ? CalculateFannedProfile(index, cardReference, ActiveCardsInHand.Num()) : CalculateHorizontalProfile(index, cardReference, ActiveCardsInHand.Num());
}

void UCardManager::HighlightPlayableCardsInHand()
{
	if (OwnerPlayerController->bTurnActive)
	{
		for (UCardWidget* each : ActiveCardsInHand)
		{
			FCardData data = UDeckFunctionLibrary::GetCardData(each->CardName, ECardSet::CardSet_Empty);
			each->OnHighlightCard.Broadcast(OwnerPlayerController->bTurnActive ? OwnerPlayerState->Mana >= data.PlacementSettings.Cost : false);
		}
	}
	else
	{
		for (UCardWidget* each : ActiveCardsInHand)
		{
			each->OnHighlightCard.Broadcast(false);
		}
	}
}

/* Position Profile */
FWidgetTransform UCardManager::CalculateHorizontalProfile(int32 cardIndex, UCardWidget* cardReference, int32 cardsInHand)
{
	//TODO: MATHEMATICS HERE
	return UCardLayoutFunctionLibrary::Horizontal(cardReference, cardIndex, cardsInHand, MaxCards, ScreenRes, CardSize, bFlyInDirection, bRotateToOffset, RadiusOffset, RotationDistanceScaleFactor, CardSpacing, bDynamicSpacing, DynamicSpacingMaxOffset, ScreenOffsetX, ScreenOffsetY, CardScale, ViewportScale, CardOffset);
}

FWidgetTransform UCardManager::CalculateFannedProfile(int32 cardIndex, UCardWidget* cardReference, int32 cardsInHand)
{
	//TODO: MATHEMATICS HERE
	return UCardLayoutFunctionLibrary::Fanned(cardReference, cardIndex, cardsInHand, MaxCards, UWidgetLayoutLibrary::GetViewportSize(this), CardSize, bFlyInDirection, bRotateToOffset, CardOffset, RadiusOffset, RotationDistanceScaleFactor, CardSpacing, bDynamicSpacing, DynamicSpacingMaxOffset, ScreenOffsetX, ScreenOffsetY, UWidgetLayoutLibrary::GetViewportScale(this), CardScale);
}

void UCardManager::SetLayout(FName rowName)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CardLayoutProfiles(TEXT("DataTable'/Game/Data/Gameplay/CardLayoutProfiles_DataTable.CardLayoutProfiles_DataTable'"));
	if (CardLayoutProfiles.Succeeded())
	{
		FCardLayout layout;
		if (UDataTableFunctionLibrary::GetDataTableRowFromName(CardLayoutProfiles.Object, rowName, layout))
		{
			RadiusOffset = layout.RadialOffset;
			CardSpacing = layout.SpacingMin;
			CardOffset = layout.Offset;

			ScreenOffsetX = (UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.X, true) * FMath::Clamp(layout.ScreenOffsetX / 100.0f, 0.0f, 1.0f)) - (UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.X, true) / 2.0f);
			ScreenOffsetY = 4.0f * ((UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true) * FMath::Clamp((layout.ScreenOffsetY / 100.0f), 0.0f, 1.0f)) - (UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true) / 4.0f));

			RotationDistanceScaleFactor = layout.RotationScaleFactor;
			MaxCards = layout.MaxCardsInHand;
			bFlyInDirection = layout.FlyInDirection;
			TransformPivot = layout.TransformPivot;
			CardScale = layout.Scale;
			bRotateToOffset = layout.RotateCardsToOffset;
			PositioningProfile = layout.PositioningProfile;
			bDynamicSpacing = layout.DynamicSpacing;
			DynamicSpacingMaxOffset = layout.SpacingMax;
		}
	}
}

void UCardManager::DarkenBackground(bool enable)
{
	bDarkenBackground = enable;
	BackgroundDarken_Image->SetVisibility(bDarkenBackground ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

/*INNER EVENTS*/
void UCardManager::PositioningFeaturesButtonClicked()
{
	WidgetSwitcher_42->SetActiveWidgetIndex(0);
	PositioningFeatures_Button->SetIsEnabled(false);
	CardsAndDeck_Button->SetIsEnabled(true);
}
void UCardManager::CardsAndDeckButtonClicked()
{
	WidgetSwitcher_42->SetActiveWidgetIndex(1);
	PositioningFeatures_Button->SetIsEnabled(true);
	CardsAndDeck_Button->SetIsEnabled(false);
}

float UCardManager::RadiusSliderValueBound()
{
	return ((RadiusOffset * -1.0f) * 5.0f);
}
void UCardManager::RadiusSliderOnValueChanged(float value)
{
	RadiusOffset = ((value / 5.0f) * -1.0f);
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

float UCardManager::PosOffsetSliderValueBound()
{
	return CardSpacing / 1000.0f;
}
void UCardManager::PosOffsetSliderOnValueChanged(float value)
{
	CardSpacing = value * 1000.0f;
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

float UCardManager::RotationDistanceOffsetSliderValueBound()
{
	return RotationDistanceScaleFactor;
}
void UCardManager::RotationDistanceOffsetSliderOnValueChanged(float value)
{
	RotationDistanceScaleFactor = value;
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

float UCardManager::CardScaleSliderValueBound()
{
	return CardScale;
}
void UCardManager::CardScaleSliderOnValueChanged(float value)
{
	CardScale = value;
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

float UCardManager::CardOffsetSliderValueBound()
{
	return (CardOffset / 20.0f) + 0.5f;
}
void UCardManager::CardOffsetSliderOnValueChanged(float value)
{
	CardOffset = FMath::FloorToFloat(20.0f * (value - 0.5f));
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

FText UCardManager::RadiusTextBoxValueBound()
{
	return UKismetTextLibrary::Conv_FloatToText(RadiusOffset, ERoundingMode::HalfToEven);
}
void UCardManager::RadiusTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
	RadiusOffset = UKismetStringLibrary::Conv_StringToFloat(text.ToString());
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

FText UCardManager::CardSpacingTextBoxValueBound()
{
	return UKismetTextLibrary::Conv_FloatToText(CardSpacing, ERoundingMode::HalfToEven);
}
void UCardManager::CardSpacingTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
	CardSpacing = UKismetStringLibrary::Conv_StringToFloat(text.ToString());
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

FText UCardManager::RotationDistanceOffsetTextBoxValueBound()
{
	return UKismetTextLibrary::Conv_FloatToText(RotationDistanceScaleFactor, ERoundingMode::HalfToEven);
}
void UCardManager::RotationDistanceOffsetTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
	RotationDistanceScaleFactor = UKismetStringLibrary::Conv_StringToFloat(text.ToString());
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

FText UCardManager::CardScaleTextBoxValueBound()
{
	return UKismetTextLibrary::Conv_FloatToText(CardScale, ERoundingMode::HalfToEven);
}
void UCardManager::CardScaleTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
	CardScale = UKismetStringLibrary::Conv_StringToFloat(text.ToString());
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

FText UCardManager::CardOffsetTextBoxValueBound()
{
	return UKismetTextLibrary::Conv_FloatToText(CardOffset, ERoundingMode::HalfToEven);
}
void UCardManager::CardOffsetTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
	CardOffset = UKismetStringLibrary::Conv_StringToFloat(text.ToString());
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

float UCardManager::OffsetXSliderValueBound()
{
	return (ScreenOffsetX + (UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.X, true) / 2.0f)) / UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.X, true);
}
void UCardManager::OffsetXSliderOnValueChanged(float value)
{
	ScreenRes = UWidgetLayoutLibrary::GetViewportSize(this);
	ScreenOffsetX = UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.X, true) * (value - 0.5f);
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

float UCardManager::OffsetYSliderValueBound()
{
	return ((ScreenOffsetY / 4.0f) + (UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true) / 4.0f) / UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true));
}
void UCardManager::OffsetYSliderOnValueChanged(float value)
{
	ScreenRes = UWidgetLayoutLibrary::GetViewportSize(this);
	ScreenOffsetY = ((UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true) * value) - (UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true) / 4.0f)) * 4.0f;
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

FText UCardManager::OffsetXTextBoxValueBound()
{
	return FText::FromString(FString::FromInt(FMath::FloorToInt(((ScreenOffsetX / UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.X, true)) * 100.0f) + 50.0f)).Append("%"));
}
void UCardManager::OffsetXTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
}

FText UCardManager::OffsetYTextBoxValueBound()
{
	return FText::FromString(FString::FromInt(FMath::FloorToInt((((ScreenOffsetY / 4.0f) / UMiscFunctionLibrary::ModifyDPIScaling(this, ScreenRes.Y, true)) * 100.0f) + 25.0f)).Append("%"));
}
void UCardManager::OffsetYTextBoxOnTextCommited(const FText& text, ETextCommit::Type commitMethod)
{
}

ECheckBoxState UCardManager::RotateCardsCheckBoxValueBound()
{
	return bRotateToOffset ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void UCardManager::RotateCardsCheckBoxOnCheckedStateChanged(bool bIsChecked)
{
	bRotateToOffset = bIsChecked;
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

ECheckBoxState UCardManager::LeftToRightCheckBoxValueBound()
{
	return bFlyInDirection ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void UCardManager::LeftToRightCheckBoxOnCheckedStateChanged(bool bIsChecked)
{
	bFlyInDirection = bIsChecked;
	bDynamicSpacing = false;
	OnUpdateCardPositions.Broadcast();
}

UWidget* UCardManager::PositioningProfile_ComboBox_OnGenerateWidget(FString item)
{
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/ComboBoxOptionBP_C.ComboBoxOptionBP_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UComboBoxOption>())
	{
		UComboBoxOption* result = CreateWidget<UComboBoxOption>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
		result->TextBlock->SetText(FText::FromString(item));
		return result;
	}
	return nullptr;
}
void UCardManager::PositioningProfile_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType)
{
	if (selectedItem == "Fanned")
	{
		SetLayout("Fanned_Left_Default");
		PositioningProfile = EPositioningProfile::PositioningProfile_Fanned;
	}
	else if (selectedItem == "Horizontal")
	{
		SetLayout("Horizontal_Default");
		PositioningProfile = EPositioningProfile::PositioningProfile_Horizontal;
	}
	OnUpdateCardPositions.Broadcast();
}

UWidget* UCardManager::LayoutProfile_ComboBox_OnGenerateWidget(FString item)
{
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/ComboBoxOptionBP_C.ComboBoxOptionBP_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UComboBoxOption>())
	{
		UComboBoxOption* result = CreateWidget<UComboBoxOption>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
		result->TextBlock->SetText(FText::FromString(item));
		return result;
	}
	return nullptr;
}
void UCardManager::LayoutProfile_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType)
{
	SetLayout(UKismetStringLibrary::Conv_StringToName(selectedItem));
	if (UKismetStringLibrary::StartsWith(selectedItem, "Horizontal"))
	{
		PositioningProfile = EPositioningProfile::PositioningProfile_Horizontal;
	}
	else
	{
		PositioningProfile = EPositioningProfile::PositioningProfile_Fanned;
	}
	OnUpdateCardPositions.Broadcast();
}

void UCardManager::ResetProfile_ButtonClicked()
{
	switch (PositioningProfile)
	{
	case EPositioningProfile::PositioningProfile_Fanned:
		SetLayout("Fanned_Left_Default");
		break;
	case EPositioningProfile::PositioningProfile_Horizontal:
		SetLayout("Horizontal_Default");
		break;
	default:
		break;
	}

	OnUpdateCardPositions.Broadcast();
}

void UCardManager::ResetDeck_ButtonClicked()
{
	OwnerPlayerController->OnReshuffleDeck.Broadcast();
}
void UCardManager::ClearHand_ButtonClicked()
{
	for (UCardWidget* each : ActiveCardsInHand)
	{
		each->RemoveFromParent();
	}
	OwnerPlayerController->OnClearCardsInHand.Broadcast();
	ActiveCardsInHand.Empty();
}

void UCardManager::AddCard_ButtonClicked()
{
	if (EditorCardName != "None")
	{
		OwnerPlayerController->OnDeveloper_AddCardToHand.Broadcast(EditorCardName);
	}
}

UWidget* UCardManager::CardList_ComboBox_OnGenerateWidget(FString item)
{
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/ComboBoxOptionBP_C.ComboBoxOptionBP_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UComboBoxOption>())
	{
		UComboBoxOption* result = CreateWidget<UComboBoxOption>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
		result->TextBlock->SetText(FText::FromString(item));
		return result;
	}
	return nullptr;
}
void UCardManager::CardList_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType)
{
	EditorCardName = UKismetStringLibrary::Conv_StringToName(selectedItem);
}

UWidget* UCardManager::CardSet_ComboBox_OnGenerateWidget(FString item)
{
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/ComboBoxOptionBP_C.ComboBoxOptionBP_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UComboBoxOption>())
	{
		UComboBoxOption* result = CreateWidget<UComboBoxOption>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
		result->TextBlock->SetText(FText::FromString(item));
		return result;
	}
	return nullptr;
}
void UCardManager::CardSet_ComboBox_OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectionType)
{
}

void UCardManager::PickupCard_ButtonClicked()
{
	if (EditorCardName != "None")
		OwnerPlayerController->OnDeveloper_AddCardToHand.Broadcast(EditorCardName);
}

ECheckBoxState UCardManager::SkipCardCost_CheckBoxValueBound()
{
	return ECheckBoxState();
}
void UCardManager::SkipCardCost_CheckBoxOnCheckedStateChanged(bool bIsChecked)
{
	OwnerPlayerController->OnSetSkipManaCheck.Broadcast(bIsChecked);
}

void UCardManager::ToggleGameUIButtonClicked()
{
	OwnerPlayerController->PlayerGameUIRef->ToggleGameUI();
}

void UCardManager::EditorButtonClicked()
{
	this->ToggleDisplayCardEditor();
}

FEventReply UCardManager::On_BackgroundDarken_Image_MouseButtonDown(FGeometry InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bEditorDisplayed)
		OnToggleDisplayCardEditor.Broadcast();

	return FEventReply(true);
}
