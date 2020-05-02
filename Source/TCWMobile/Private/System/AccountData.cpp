// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "AccountData.h"

UAccountData::UAccountData(const FObjectInitializer& ObjectInitializer) : USaveGame(ObjectInitializer)
{
	//PRE - 01
	SaveSlotName = TEXT("AccountSaveSlot");
	UserIndex = 0;
}