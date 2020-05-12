// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

#include "Enums.h"
#include "TCWPlayerController.h"
#include "UIEnums.h"
#include "ViewCard.h"

#include "CardWidget.generated.h"

class UCardManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCardWidgetEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardHighlightEvent, bool, enable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCardOpenGateEvent, FWidgetTransform, goalTransform, UWidget*, moveComponent, float, interpSpeed, ECardWidgetMovementState, requestedState);

/**
 *
 */
UCLASS()
class TCWMOBILE_API UCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, Category = "Card Events")
		FCardWidgetEvent OnSetDefaults;
	UPROPERTY(BlueprintCallable, Category = "Card Events")
		FCardWidgetEvent OnForceMouseLeave;
	UPROPERTY(BlueprintCallable, Category = "Card Events")
		FCardWidgetEvent OnViewCardFocusLost;
	UPROPERTY(BlueprintCallable, Category = "Card Events")
		FCardOpenGateEvent OnOpenGate;
	UPROPERTY(BlueprintCallable, Category = "Card Events")
		FCardWidgetEvent OnCloseGate;
	UPROPERTY(BlueprintCallable, Category = "Card Events")
		FCardHighlightEvent OnHighlightCard;
	
public:
	UCardManager* UIParent;
	bool bIsFaceUp;

	FName CardName;
	ECardSet CardSetEnum;

	FWidgetTransform HomeTransform;

protected:
	FWidgetTransform PreviewLocationCenter;

	ATCWPlayerController* OwningPlayerRef;
	UCardManager* CardManagerRef;
	UWidget* MoveComponent;
	bool bPlatformMobile;
	bool bIsDragDetected;
	bool bIsValidDrop;
	bool bCanDrag;
	bool bIsMouseDown;
	bool bIsMouseOver;
	bool bInMobilePreview;
	bool bIsMoving;
	float DeltaTime;
	float InterpSpeed;
	EViewState CardViewState;
	FVector2D ViewportSize;

	FWidgetTransform GoalTransform;
	FWidgetTransform ViewCardInitTransform;
	ECardWidgetMovementState CardMovementState;

public:
	UCardWidget(const FObjectInitializer& ObjectInitializer);
	void SetFaceDown(bool enableFacedown);
	void SetCardData();

private:
	UFUNCTION()
		void SetDefaults();
	UFUNCTION()
		void OpenGate(FWidgetTransform goalTransform, UWidget* moveComponent, float interpSpeed, ECardWidgetMovementState requestedState);
	UFUNCTION()
		void CloseGate();
	UFUNCTION()
		void ForceMouseLeave();
	UFUNCTION()
		void ViewCardFocusLost();
	UFUNCTION()
		void HighlightCard(bool enable);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* CardCanvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* MouseOverHitBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UViewCard* ViewCard;

	UDragDropOperation* DragDropAction;

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

private:
	void EnableMobileCardPreview(bool enablePreview);
	void EnableInHandPreview();
	void DisableInHandPreview(bool forceHomeLocation);

	bool ValidateCardMovementRequest(ECardWidgetMovementState requestedState);
	bool ValidateMouseOverPreview();
};
