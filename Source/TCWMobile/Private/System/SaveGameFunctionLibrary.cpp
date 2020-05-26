// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "SaveGameFunctionLibrary.h"
#include "Account/CustomDeckSave.h"
#include "Account/TCWSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USaveGameFunctionLibrary::RemoveSaveGameSlot(FString saveGame)
{
	//TODO: USaveGameFunctionLibrary::RemoveSaveGameSlot
}

USaveGame* USaveGameFunctionLibrary::CreateLoadCardGameSave(FString slotName, TSubclassOf<USaveGame> saveGameClass, bool& success)
{
	USaveGame* result = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		result = UGameplayStatics::LoadGameFromSlot(slotName, 0);
		success = true;
	}
	else
	{
		result = UGameplayStatics::CreateSaveGameObject(saveGameClass);
		success = UGameplayStatics::SaveGameToSlot(result, slotName, 0);
	}
	return result;
}

bool USaveGameFunctionLibrary::SaveCustomDeck(FText inText, TArray<FName> customDeck, bool editable)
{
	const FString slotName = "CardGameSave";

	if (!UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		bool isSuccess;
		CreateLoadCardGameSave(slotName, UTCWSaveGame::StaticClass(), isSuccess);
	}

	if (UTCWSaveGame* loaded = Cast<UTCWSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 0)))
	{
		if (!(loaded->DeckList.Find(inText.ToString()) >= 0))
		{
			loaded->DeckList.Add(inText.ToString());
			UGameplayStatics::SaveGameToSlot(loaded, slotName, 0);

			bool isSuccess;
			CreateLoadCardGameSave(slotName, UCustomDeckSave::StaticClass(), isSuccess);
		}

		if (UCustomDeckSave* deckLoaded = Cast<UCustomDeckSave>(UGameplayStatics::LoadGameFromSlot(slotName, 0)))
		{
			deckLoaded->CustomDeck = customDeck;
			deckLoaded->bEditable = editable;

			return UGameplayStatics::SaveGameToSlot(deckLoaded, inText.ToString(), 0);
		}
	}
	
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

