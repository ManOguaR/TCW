// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "GameUI.h"
#include "CardManager.h"
#include "GameProfileUI.h"
#include "MessageManager.h"
#include "TCWGameState.h"
#include "TCWPlayerState.h"
#include "TCWPlayerController.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetStringLibrary.h"

UGameUI::UGameUI(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnToggleGameUI.AddDynamic(this, &UGameUI::ToggleGameUI);
	OnUpdateGameAndTurnTimers.BindDynamic(this, &UGameUI::UpdateGameAndTurnTimers);
	OnCheckTurnBeginState.BindDynamic(this, &UGameUI::CheckTurnBeginState);
}

UCardManager* UGameUI::GetCardManager()
{
	return CardManager;
}

void UGameUI::UpdateUIPlayerStats_Implementation(bool forceCleanVisualUpdate)
{
	if (PlayerStateRef && PlayerStateRef->IsValidLowLevel())
	{
		PlayerProfileUI->OnUpdatePlayerUI.Broadcast(PlayerStateRef);
	}
}
void UGameUI::UpdateUITurnState_Implementation(bool turnActive, EGameTurn turnState)
{
	currentTurnState = turnState;
	TurnButton->SetBackgroundColor(currentTurnState == EGameTurn::GameTurn_Active ? FLinearColor(0.1f, 1.0f, 0.0f) : (currentTurnState == EGameTurn::GameTurn_Inactive ? FLinearColor(1.0f, 0.0f, 0.05f) : FLinearColor(1.0f, 0.45f, 0.0f)));
	UpdateGameAndTurnTimers();
}
//void UGameUI::UpdateUIGameTurnTime_Implementation()
//{
//
//}
//void UGameUI::UpdatePlayerStateUI_Implementation(int32 p1, int32 p2, int32 p3, int32 p4)
//{
//
//}

void UGameUI::ToggleGameUI()
{
	//FLIP FLOP
	bIsVisible = !bIsVisible;
	if (bIsVisible)
	{
		PlayerUI_Canvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MessageManager->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TurnButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PlayerUI_Canvas->SetVisibility(ESlateVisibility::Hidden);
		MessageManager->SetVisibility(ESlateVisibility::Hidden);
		TurnButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameUI::OnSpawnAIOpponent_ButtonClicked()
{
	CardManager->OwnerPlayerController->OnSpawnAIOpponent.Broadcast();
}

void UGameUI::OnToggleMessageManager_ButtonClicked()
{
	//FLIP FLOP
	bIsMessageManagerVisible = !bIsMessageManagerVisible;
	if (bIsMessageManagerVisible)
	{
		MessageManager->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MessageManager->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameUI::OnTurnButtonClicked()
{
	if (ATCWPlayerController* pc = Cast<ATCWPlayerController>(GetOwningPlayer()))
	{
		pc->OnServerRequestChangeTurnState.Broadcast();
		PlayAnimation(ChangeTurn);
	}
}

void UGameUI::UpdateGameAndTurnTimers()
{
	TurnButton_Text->SetText(FText::FromString(currentTurnState == EGameTurn::GameTurn_Active ? UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(TEXT("End Turn ("), CombineTimer(GameStateRef->TurnTimeSeconds, GameStateRef->TurnTimeMinutes)), TEXT(")")) : (currentTurnState == EGameTurn::GameTurn_Inactive ? TEXT("Enemy Turn") : TEXT("Waiting..."))));
	GameTime->SetText(FText::FromString(CombineTimer(GameStateRef->GameTimeSeconds, GameStateRef->GameTimeMinutes)));
	CardManager->HighlightPlayableCardsInHand();
}

void UGameUI::CheckTurnBeginState()
{
	if (GameStateRef->PlayerStateArray.Num() > 1)
	{
		WaitingOnPlayer_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
		UKismetSystemLibrary::K2_ClearTimerHandle(this, checkTurnTimerHandle);
	}
}

void UGameUI::NativePreConstruct()
{
	SpawnAIOpponent_Button->OnClicked.AddDynamic(this, &UGameUI::OnSpawnAIOpponent_ButtonClicked);
	TurnButton->OnClicked.AddDynamic(this, &UGameUI::OnTurnButtonClicked);
	ToggleMessageManager_Button->OnClicked.AddDynamic(this, &UGameUI::OnToggleMessageManager_ButtonClicked);
}

void UGameUI::NativeConstruct()
{
	GameStateRef = Cast<ATCWGameState>(UGameplayStatics::GetGameState(this));
	PlayerStateRef = Cast<ATCWPlayerState>(GetOwningPlayer()->PlayerState);

	if (GameStateRef->PlayerStateArray.Num() > 1)
	{
		WaitingOnPlayer_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		WaitingOnPlayer_VerticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimation(SpawnAIOpponent);
	}

	updateGameTimerHandle = UKismetSystemLibrary::K2_SetTimerDelegate(OnUpdateGameAndTurnTimers, 0.2f, true);
	checkTurnTimerHandle = UKismetSystemLibrary::K2_SetTimerDelegate(OnCheckTurnBeginState, 0.2f, true);
}

FString UGameUI::CombineTimer(int32 seconds, int32 minutes)
{
	return UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(FString::FromInt(minutes), TEXT(":")), seconds <= 9 ? UKismetStringLibrary::Concat_StrStr(TEXT("0"), FString::FromInt(seconds)) : FString::FromInt(seconds));
}
