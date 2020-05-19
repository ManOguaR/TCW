// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "CardFunctionLibrary.h"
#include "BoardUnit.h"

ABoardUnit* UCardFunctionLibrary::SetupUnit(ABoardUnit* boardUnitRef, int32 owningPlayerId, FName cardName, ECardSet cardSet, FCardData dataStruct, bool setUnitDataFromStruct)
{
	boardUnitRef->SetOwningPlayerID(owningPlayerId);
	boardUnitRef->SetReplicates(true);
	boardUnitRef->OnServerSetUnitData.Broadcast(cardName, cardSet, dataStruct, setUnitDataFromStruct);
	return boardUnitRef;
}
