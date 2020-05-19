// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "GameProfileUI.h"
#include "ResourcesBar.h"
#include "TCWPlayerState.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

UGameProfileUI::UGameProfileUI(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnUpdatePlayerUI.AddDynamic(this, &UGameProfileUI::UpdatePlayerUI);
}

void UGameProfileUI::UpdatePlayerUI(ATCWPlayerState* playerStateRef)
{
	if (!PlayerStateRef || !PlayerStateRef->IsValidLowLevel())
	{
		PlayerStateRef = playerStateRef;

		//TODO: SetBrushFromMaterial
		//Player_Image->SetBrushFromMaterial((PlayerStateRef->GamePlayerId - 1) == 0 ? () : ());

		PlayerName_Text->SetText(FText::FromString(PlayerStateRef->GetPlayerName()));
	}

	PlayerHealth_Text->SetText(FText::FromString(FString::FromInt(PlayerStateRef->Health)));

	CardsInDeck_Text->SetText(FText::FromString(FString::FromInt(PlayerStateRef->CardsInDeck)));

	PlayerTurn_Text->SetText(FText::FromString(FString::FromInt(PlayerStateRef->PlayerTurn)));

	Mana_Text->SetText(FText::FromString(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(FString::FromInt(PlayerStateRef->Mana), TEXT("/")), FString::FromInt(PlayerStateRef->Mana_Max))));

	ResourcesBar->OnUpdateManaVisual.Broadcast(PlayerStateRef);
}
