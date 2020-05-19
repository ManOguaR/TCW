// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "Graveyard.h"
#include "BoardUnit.h"
#include "CardFunctionLibrary.h"
#include "ControllerFunctionLibrary.h"
#include "DeckFunctionLibrary.h"

// Sets default values
AGraveyard::AGraveyard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnAddToGraveyard.AddDynamic(this, &AGraveyard::AddToGraveyard);
	OnAddToGraveyardVisual.AddDynamic(this, &AGraveyard::AddToGraveyardVisual);
	OnRemoveCardFromGraveyard.AddDynamic(this, &AGraveyard::RemoveCardFromGraveyardEvent);
}

// Called when the game starts or when spawned
void AGraveyard::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGraveyard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGraveyard::AddToGraveyard(ABoardUnit* boardUnit, FName name)
{
	if (HasAuthority())
	{
		if (boardUnit && boardUnit->IsValidLowLevel())
		{
			ActorsInGraveyard.AddUnique(boardUnit);
			GraveyardList.Add(boardUnit->CardDataTableName);
			GraveyardStructList.Add(boardUnit->CardDataStruct);
		}
		else
		{
			GraveyardList.Add(name);
			GraveyardStructList.Add(UDeckFunctionLibrary::GetCardData(name, ECardSet::CardSet_Empty));
		}
		HideUnseenCardsInGraveyard();
	}

}

void AGraveyard::AddToGraveyardVisual()
{

}

void AGraveyard::RemoveCardFromGraveyardEvent(FName card, int32 index)
{
	if (HasAuthority())
	{
		RemoveCardFromGraveyard(card, index);
		RecreateTopGraveyardCardActor();
	}
}
void AGraveyard::RemoveCardFromGraveyard(FName card, int32 index)
{
	RemoveIndex = index;

	if ((GraveyardList.Num() > 0) && GraveyardList[RemoveIndex] == card)
	{
		GraveyardStructList.RemoveAt(RemoveIndex);
		if (ActorsInGraveyard[0]->IsValidLowLevel())
		{
			ActorsInGraveyard[0]->OnSetUnitHiddenState.Broadcast(true);
			ActorsInGraveyard[0]->OnRemoveUnitActor.Broadcast();
			ActorsInGraveyard.RemoveAt(0);
			GraveyardList.RemoveAt(RemoveIndex);
		}
	}
}

void AGraveyard::HideUnseenCardsInGraveyard()
{
	if (ActorsInGraveyard.Num() > 1)
	{
		int32 arrayIndex = 0;
		for (ABoardUnit* each : ActorsInGraveyard)
		{
			if (each->IsValidLowLevel())
			{
				if ((ActorsInGraveyard.Num() - 1) != arrayIndex)
				{
					each->OnRemoveUnitActor.Broadcast();
					ActorsInGraveyard.Remove(each);
				}
			}
			arrayIndex++;
		}
	}
}

void AGraveyard::RecreateTopGraveyardCardActor()
{
	if ((HasAuthority()) && (GraveyardList.Num() > 0) && (ActorsInGraveyard.Num() == 0))
	{
		ABoardUnit* unit = Cast<IControllerInterface>(UControllerFunctionLibrary::GetControllerReferenceFromID(PlayerIndex))->CreatePlayableUnit(FTransform(GetUnitInGraveyardLocation(1)));

		ABoardUnit* unitRef = UCardFunctionLibrary::SetupUnit(unit, PlayerIndex, GraveyardList[GraveyardList.Num()], ECardSet::CardSet_Empty, GraveyardStructList[GraveyardList.Num()], true);

		ActorsInGraveyard.AddUnique(unitRef);

		unitRef->OnForceMoveCardDirectlyToGraveyard.Broadcast(this);
	}
}

FVector AGraveyard::GetUnitInGraveyardLocation(int32 add)
{
	FVector location = GetActorLocation();
	return FVector(location.X, location.Y, (location.Z + ((1.5f * (float)(GraveyardList.Num() + add)) + 10.0f)));
}
