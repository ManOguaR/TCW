// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "SaveGameFunctionLibrary.h"

void USaveGameFunctionLibrary::RemoveSaveGameSlot(FString saveGame)
{
	//TODO: USaveGameFunctionLibrary::RemoveSaveGameSlot
}

bool USaveGameFunctionLibrary::CreateLoadCardGameSave(FString slotName, TSubclassOf<USaveGame> saveGameClass, USaveGame* saveGame)
{
	//TODO: USaveGameFunctionLibrary::CreateLoadCardGameSave
	return false;
}

bool USaveGameFunctionLibrary::SaveCustomDeck(FText& inText, TArray<FName> customDeck, bool editable)
{
	//TODO: USaveGameFunctionLibrary::SaveCustomDeck
	return false;
}

TArray<FName> USaveGameFunctionLibrary::LoadCustomDeck(FString deckName, bool& deckEditable, bool& deckValid)
{
	//TODO: USaveGameFunctionLibrary::LoadCustomDeck
	return TArray<FName>();
}

FString USaveGameFunctionLibrary::GetRandomDeck(bool& deckValid)
{
	//TODO: USaveGameFunctionLibrary::GetRandomDeck
	return FString();
}

bool USaveGameFunctionLibrary::GetDeckValid(FString slotName)
{
	//TODO: USaveGameFunctionLibrary::GetDeckValid
	return false;
}

bool USaveGameFunctionLibrary::GetDeckEditable(FString slotName)
{
	//TODO: USaveGameFunctionLibrary::GetDeckEditable
	return false;
}

