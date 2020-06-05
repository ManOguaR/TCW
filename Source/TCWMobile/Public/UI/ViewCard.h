// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "BoardUnit.h"
#include "Enums.h"

#include "ViewCard.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FViewCardEvent);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FViewCardInitEvent, FName, name, ECardSet, cardRace, UUserWidget*, callingWidgetParent, ABoardUnit*, unitParent, bool, isPreview);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewCardFacedownEvent, bool, enableFacedown);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewCardHaloEvent, ESlateVisibility, cardVisibility, FLinearColor, inColorAndOpacity);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewCardDataEvent, FName, cardName, ECardSet, cardSet);

/**
 *
 */
UCLASS()
class TCWMOBILE_API UViewCard : public UUserWidget
{
	GENERATED_BODY()

public:
	UViewCard(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "System")
		bool bIsFaceUp;

protected:
	FVector2D CardSize;
	FVector2D ViewportSize;
	bool bIsPreview;

	UUserWidget* CardWidgetParent;
	ABoardUnit* GameUnitParent;

	FName CardName;
	FSlateBrush CardBrush;
	FSlateBrush CardFrameBrush;
	FSlateBrush CardBackBrush;
	ECardSet CardSetEnum;

	void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Card Events")
		void InitiateCard(FName name, ECardSet cardRace, UUserWidget* callingWidgetParent, ABoardUnit* unitParent, bool isPreview);
	UFUNCTION(BlueprintCallable, Category = "Card Events")
		void SetFacedown(bool enableFacedown);
	UFUNCTION(BlueprintCallable, Category = "Card Events")
		void SetCardHalo(ESlateVisibility cardVisibility, FLinearColor inColorAndOpacity);
	UFUNCTION(BlueprintCallable, Category = "Card Events")
		void SetCardData(FName cardName, ECardSet cardSet);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* CardTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* DescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* AttackTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* HealthTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* CostTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CardFrame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CardImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CostIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* HealthIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* AttackIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* ShadowImage;
};
