// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "CardWidget.h"
#include "CardManager.h"
#include "MiscFunctionLibrary.h"
#include "TCWGameInstance.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

UCardWidget::UCardWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnSetDefaults.AddDynamic(this, &UCardWidget::SetDefaults);
	OnForceMouseLeave.AddDynamic(this, &UCardWidget::ForceMouseLeave);
	OnViewCardFocusLost.AddDynamic(this, &UCardWidget::ViewCardFocusLost);

	OnOpenGate.AddDynamic(this, &UCardWidget::OpenGate);
	OnCloseGate.AddDynamic(this, &UCardWidget::CloseGate);

	OnHighlightCard.AddDynamic(this, &UCardWidget::HighlightCard);
}

void UCardWidget::SetDefaults()
{
	bIsDragDetected = false;
	bInMobilePreview = false;
	bIsMouseDown = false;

	ViewCard->SetVisibility(bPlatformMobile ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible);

	MouseOverHitBox->SetVisibility(ESlateVisibility::Visible);
}

void UCardWidget::ForceMouseLeave()
{
	if (!bPlatformMobile && !bIsDragDetected)
		DisableInHandPreview(false);
}

void UCardWidget::ViewCardFocusLost()
{
	bIsMouseDown = false;
	EnableMobileCardPreview(false);
}

void UCardWidget::HighlightCard(bool enable)
{
	ViewCard->OnSetCardHalo.Broadcast(ESlateVisibility::Visible, enable ? FLinearColor(0.128829f, 1.0f, 0.0f, 0.5f) : FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
}

void UCardWidget::OpenGate(FWidgetTransform goalTransform, UWidget* moveComponent, float interpSpeed, ECardWidgetMovementState requestedState)
{
	if (!bIsMoving || (bIsMoving && ValidateCardMovementRequest(requestedState)))
	{
		CardMovementState = requestedState;
		GoalTransform = goalTransform;
		MoveComponent = moveComponent;
		InterpSpeed = interpSpeed;
		bIsMoving = true;
	}
}

void UCardWidget::CloseGate()
{
	bIsMoving = false;
}

void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OwningPlayerRef = Cast<ATCWPlayerController>(GetOwningPlayer());

	CardManagerRef = OwningPlayerRef->PlayerGameUIRef->GetCardManager();
	ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	PreviewLocationCenter = FWidgetTransform(FVector2D(0.0f, 120.0f), FVector2D(1.2f, 1.2f), FVector2D(0.0f), 0.0f);

	ViewCardInitTransform = ViewCard->RenderTransform;

	Cast<UTCWGameInstance>(GetGameInstance())->GetCurrentPlatform(bPlatformMobile);

	if (OnSetDefaults.IsBound())
		OnSetDefaults.Broadcast();
}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	DeltaTime = InDeltaTime;

	if (bIsMoving)
	{
		bool arrived;
		MoveComponent->SetRenderTransform(UMiscFunctionLibrary::InterpToGoalLocation2D(MoveComponent->RenderTransform, GoalTransform, DeltaTime, InterpSpeed, arrived));
		if (arrived)
		{
			if (OnCloseGate.IsBound())
				OnCloseGate.Broadcast();

			CardMovementState = ECardWidgetMovementState::CardMovementState_Pending;
		}
	}
}

void UCardWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	ViewCardFocusLost();
}

FReply UCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsMouseDown = true;

	if (OnForceMouseLeave.IsBound())
		OnForceMouseLeave.Broadcast();

	return FReply::Handled();
}

FReply UCardWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsMouseDown = false;

	return FReply::Handled();
}

FReply UCardWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsMouseDown)
	{
		if (ATCWPlayerController* pc = Cast<ATCWPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			if (pc->bTurnActive && CardManagerRef->CardViewState == CardManagerRef->DraggableCardViewState)
			{
				return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
			}
		}
	}
	return FReply::Handled();
}

void UCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	bIsMouseOver = true;
	if (!bPlatformMobile)
	{
		UKismetSystemLibrary::Delay(this, 0.2f, FLatentActionInfo());
		if (ValidateMouseOverPreview())
			EnableInHandPreview();
	}
}

void UCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bIsMouseOver = false;
	ForceMouseLeave();
}

void UCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	bIsDragDetected = true;
	OnCloseGate.Broadcast();
	ViewCard->SetVisibility(ESlateVisibility::Collapsed);
	MouseOverHitBox->SetVisibility(ESlateVisibility::Collapsed);

	if (bPlatformMobile)
	{
		if (bInMobilePreview)
		{
			CardManagerRef->SetCardViewState(EViewState::ViewState_Default, false);
		}
	}
	else
	{
		DisableInHandPreview(true);
	}

	DragDropAction = NewObject<UDragDropOperation>();
	DragDropAction->Payload = this;
	DragDropAction->Pivot = EDragPivot::CenterCenter;
	DragDropAction->Offset = FVector2D(0.0f);
	DragDropAction->Tag = "";

	if (ATCWPlayerController* pc = Cast<ATCWPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (CardName != TEXT("None"))
		{
			pc->OnCallCreateCard.Broadcast(CardName, DragDropAction, CardManagerRef->ActiveCardsInHand.Find(this), CardSetEnum);
			if (UIParent->CardViewState == EViewState::ViewState_Default)
			{
				UIParent->SetCardViewState(EViewState::ViewState_HideHand, false);
			}
		}
		else
		{
			OnSetDefaults.Broadcast();
		}
	}

	OutOperation = DragDropAction;
}

void UCardWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	OwningPlayerRef->OnDragCancelled.Broadcast();
}

bool UCardWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bIsMouseDown = false;
	OnSetDefaults.Broadcast();

	return true;
}

FReply UCardWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	bIsMouseDown = false;

	if (!bIsDragDetected && bPlatformMobile)
	{
		switch (UIParent->CardViewState)
		{
		case EViewState::ViewState_Default:
		case EViewState::ViewState_CardInPreview:
		{
			if (bInMobilePreview)
			{
				if (ValidateCardMovementRequest(ECardWidgetMovementState::CardMovementState_Preview))
					EnableMobileCardPreview(false);
			}
			else
			{
				if (ValidateCardMovementRequest(ECardWidgetMovementState::CardMovementState_Return))
					EnableMobileCardPreview(true);
			}
			break;
		}
		case EViewState::ViewState_ViewHand:
		case EViewState::ViewState_HideHand:
		case EViewState::ViewState_TestProfile:
		default:
			break;
		}
	}

	return FReply::Handled();
}

void UCardWidget::SetFaceDown(bool enableFacedown)
{
	ViewCard->OnSetFacedown.Broadcast(enableFacedown);
}

void UCardWidget::SetCardData()
{
	ViewCard->OnInitiateCard.Broadcast(CardName, CardSetEnum, this, nullptr, false);
}

void UCardWidget::EnableMobileCardPreview(bool enablePreview)
{
	if (enablePreview != bInMobilePreview)
	{
		bInMobilePreview = !bInMobilePreview;
		UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetZOrder(bInMobilePreview ? 5 : 1);
		CardManagerRef->SetCardViewState(bInMobilePreview ? EViewState::ViewState_CardInPreview : EViewState::ViewState_Default, false);
		if (OnOpenGate.IsBound())
		{
			OnOpenGate.Broadcast(bInMobilePreview ? PreviewLocationCenter : HomeTransform, this, 15.0f, bInMobilePreview ? ECardWidgetMovementState::CardMovementState_Preview : ECardWidgetMovementState::CardMovementState_Return);
		}
	}
}

void UCardWidget::EnableInHandPreview()
{
	ViewCard->SetRenderTransformPivot(FVector2D(0.5f, 1.0f));
	UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetZOrder(5);
	if (OnOpenGate.IsBound())
	{
		OnOpenGate.Broadcast(FWidgetTransform(ViewCardInitTransform.Translation - FVector2D(0.0f, 100.0f), ViewCardInitTransform.Scale * 2.5f, ViewCardInitTransform.Shear, ViewCardInitTransform.Angle - RenderTransform.Angle), ViewCard, 20.0f, ECardWidgetMovementState::CardMovementState_Preview);
	}
}

void UCardWidget::DisableInHandPreview(bool forceHomeLocation)
{
	UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetZOrder(1);

	if (forceHomeLocation)
	{
		ViewCard->SetRenderTransform(FWidgetTransform(FVector2D(0.0f), FVector2D(1.0f), FVector2D(0.0f), 0.0f));
	}
	else
	{
		if (OnOpenGate.IsBound())
			OnOpenGate.Broadcast(FWidgetTransform(FVector2D(0.0f), FVector2D(1.0f), FVector2D(0.0f), 0.0f), ViewCard, 10.0f, ECardWidgetMovementState::CardMovementState_Return);
	}
}

bool UCardWidget::ValidateCardMovementRequest(ECardWidgetMovementState requestedState)
{
	if (requestedState == ECardWidgetMovementState::CardMovementState_Hand && (CardMovementState == ECardWidgetMovementState::CardMovementState_Preview || CardMovementState == ECardWidgetMovementState::CardMovementState_Return))
	{
		DisableInHandPreview(true);
	}
	else if (CardMovementState == ECardWidgetMovementState::CardMovementState_Hand)
	{
		return false;
	}
	return true;
}

bool UCardWidget::ValidateMouseOverPreview()
{
	return (bIsMouseOver ?
		(UWidgetBlueprintLibrary::IsDragDropping() ? false
			: (CardMovementState == ECardWidgetMovementState::CardMovementState_Hand ? false
				: (bInMobilePreview ? false
					: UIParent->CardViewState == EViewState::ViewState_Default)))
		: false);
}
