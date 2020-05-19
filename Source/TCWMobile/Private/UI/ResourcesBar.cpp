// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "ResourcesBar.h"
#include "TCWPlayerState.h"

UResourcesBar::UResourcesBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnUpdateManaVisual.AddDynamic(this, &UResourcesBar::UpdateManaVisual);
}

void UResourcesBar::UpdateManaVisual(ATCWPlayerState* playerStateRef)
{
	PlayerStateRef = playerStateRef;

	Mana_Max = PlayerStateRef->Mana_Max;
	Mana = PlayerStateRef->Mana;

	//TODO: UResourcesBar::UpdateManaVisual
}
