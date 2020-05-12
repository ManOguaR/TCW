// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "ViewCard.h"
#include "../TCWMobile.h"
#include "DeckFunctionLibrary.h"

UViewCard::UViewCard(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnInitiateCard.AddDynamic(this, &UViewCard::InitiateCard);
	OnSetFacedown.AddDynamic(this, &UViewCard::SetFacedown);
	OnSetCardHalo.AddDynamic(this, &UViewCard::SetCardHalo);
}

void UViewCard::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (CardWidgetParent->IsValidLowLevel())
		CardWidgetParent->OnFocusLost(InFocusEvent);
}

void UViewCard::InitiateCard(FName cardName, ECardSet cardRace, UUserWidget* callingWidgetParent, ABoardUnit* gameUnitParent, bool isPreview)
{
	CardName = cardName;
	CardSetEnum = cardRace;
	CardWidgetParent = callingWidgetParent;
	GameUnitParent = gameUnitParent;
	bIsPreview = isPreview;

	SetRenderTransformPivot(FVector2D(0.0f, 0.5f));
	SetCardData(CardName, CardSetEnum);
}

void UViewCard::SetFacedown(bool enableFacedown)
{
	NameTextBlock->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	CardTypeTextBlock->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	DescriptionTextBlock->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	AttackTextBlock->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	HealthTextBlock->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	CostTextBlock->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	CardImage->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	CostIcon->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	HealthIcon->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	AttackIcon->SetVisibility(enableFacedown ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	CardFrame->SetBrush(enableFacedown ? CardBackBrush : CardFrameBrush);
}

void UViewCard::SetCardHalo(ESlateVisibility visibility, FLinearColor inColorAndOpacity)
{
	ShadowImage->SetVisibility(visibility);
	ShadowImage->SetColorAndOpacity(inColorAndOpacity);
}

void UViewCard::SetCardData(FName cardName, ECardSet cardSet)
{
	FCardData data = UDeckFunctionLibrary::GetCardData(cardName, cardSet);

	NameTextBlock->SetText(data.Name);
	CardTypeTextBlock->SetText(GETENUMTEXT("ECardType", data.Type));// FText::FromString(GETENUMSTRING("ECardType", data.Type)));

	DescriptionTextBlock->SetText(FText::FromString(bIsPreview ? GameUnitParent->Description : data.Description.Description));
	AttackTextBlock->SetText(FText::FromString(FString::FromInt(data.Attack.Damage)));
	HealthTextBlock->SetText(FText::FromString(FString::FromInt(data.Health.Health)));
	CostTextBlock->SetText(FText::FromString(FString::FromInt(data.PlacementSettings.Cost)));

	CardBrush = FSlateDynamicImageBrush(data.Visual.ImageTexture, FVector2D(32.0f, 32.0f), data.Visual.ImageTexture->GetFName());
	CardFrameBrush = FSlateDynamicImageBrush(data.Visual.FrameTexture, FVector2D(32.0f, 32.0f), data.Visual.FrameTexture->GetFName());
	CardBackBrush = FSlateDynamicImageBrush(data.Visual.BackTexture, data.DeckSettings.ScreenSize, data.Visual.BackTexture->GetFName());

	CardSize = data.DeckSettings.ScreenSize;
}
