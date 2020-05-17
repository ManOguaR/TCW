// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "GameUI.h"
#include "TCWPlayerController.h"

UGameUI::UGameUI(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnToggleGameUI.AddDynamic(this, &UGameUI::ToggleGameUI);
}

UCardManager* UGameUI::GetCardManager()
{
	return CardManager;
}

void UGameUI::UpdateUIPlayerStats_Implementation(bool forceCleanVisualUpdate)
{

}
void UGameUI::UpdateUITurnState_Implementation(bool turnActive, EGameTurn turnState)
{

}
void UGameUI::UpdateUIGameTurnTime_Implementation()
{

}
void UGameUI::UpdatePlayerStateUI_Implementation(int32 p1, int32 p2, int32 p3, int32 p4)
{

}

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

void UGameUI::OnTurnButtonClicked()
{
	if (ATCWPlayerController* pc = Cast<ATCWPlayerController>(GetOwningPlayer()))
	{
		
	}
}

void UGameUI::NativePreConstruct()
{
	TurnButton->OnClicked.AddDynamic(this, &UGameUI::OnTurnButtonClicked);
}
