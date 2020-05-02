// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DeckFunctionLibrary.h"

TArray<FName> UDeckFunctionLibrary::GetChosenDeckArray(int32 index, UDataTable* target)
{
	//TODO: UDeckFunctionLibrary::GetChosenDeckArray
	return TArray<FName>();
}

FCardData UDeckFunctionLibrary::GetCardData(FName cardName)
{
	//TODO: UDeckFunctionLibrary::GetCardData
	return FCardData();
}

int32 UDeckFunctionLibrary::GetRandomCardFromDeck(AController* controller, FName& cardName)
{
	//TODO: UDeckFunctionLibrary::GetRandomCardFromDeck
	return int32();
}

FName UDeckFunctionLibrary::GetCardFromDeck(TArray<FName> deck, int32 index, bool lastIndex)
{
	//TODO: UDeckFunctionLibrary::GetCardFromDeck
	return FName();
}

int32 UDeckFunctionLibrary::FindCardInArray(FName cardName, TArray<FName> cardArray, TArray<int32> indexesArray)
{
	//TODO: UDeckFunctionLibrary::FindCardInArray
	return int32();
}

TArray<FName> UDeckFunctionLibrary::GetAllCardsInActiveSet()
{
	//TODO: UDeckFunctionLibrary::GetAllCardsInActiveSet
	return TArray<FName>();
}

void UDeckFunctionLibrary::RemoveCardFromDeck(bool removeAll, int32 index, TArray<FName> deck)
{
	//TODO: UDeckFunctionLibrary::RemoveCardFromDeck
}

int32 UDeckFunctionLibrary::CountCardsInDeck(TArray<FName> deck)
{
	//TODO: UDeckFunctionLibrary::CountCardsInDeck
	return int32();
}

TArray<FName> UDeckFunctionLibrary::FilterWeightedCardsInDeck(TArray<FName> cardsInDeck)
{
	//TODO: UDeckFunctionLibrary::FilterWeightedCardsInDeck
	return TArray<FName>();
}
