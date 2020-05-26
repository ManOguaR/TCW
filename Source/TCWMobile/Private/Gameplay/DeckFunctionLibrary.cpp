// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DeckFunctionLibrary.h"
#include "..\TCWMobile.h"

#include "Engine\DataTable.h"

TArray<FName> UDeckFunctionLibrary::GetChosenDeckArray(int32 index, UDataTable* target)
{
	//TODO: UDeckFunctionLibrary::GetChosenDeckArray
	return TArray<FName>();
}

FCardData UDeckFunctionLibrary::GetCardData(FName cardName, ECardSet cardset)
{
	ECardSet tempCardSet;
	UDataTable* dataTable = nullptr;

	switch (cardset)
	{
	case ECardSet::CardSet_Empty:
	case ECardSet::CardSet_Basic:
	{
		tempCardSet = ECardSet::CardSet_Basic;
		ConstructorHelpers::FObjectFinder<UDataTable> Table_BP(TEXT("DataTable'/Game/Data/Gameplay/DebugSet_DataTable.DebugSet_DataTable'"));
		if (Table_BP.Succeeded())
			dataTable = Table_BP.Object;
		break;
	}
	case ECardSet::CardSet_Debug:
	{
		tempCardSet = ECardSet::CardSet_Debug;
		ConstructorHelpers::FObjectFinder<UDataTable> Table_BP(TEXT("DataTable'/Game/Data/Gameplay/BasicSet_DataTable.BasicSet_DataTable'"));
		if (Table_BP.Succeeded())
			dataTable = Table_BP.Object;
		break;
	}
	case ECardSet::CardSet_EmtySet:
	default:
	{
		tempCardSet = ECardSet::CardSet_Empty;
		break;
	}
	}

	if (!dataTable) {
		UE_LOG(TCWLogErrors, Fatal, TEXT("Failed to get DataTable"));
	}

	FCardData* result = dataTable->FindRow<FCardData>(cardName, TEXT("LookUp Operation"));

	if (result) {
		return FCardData(*result);
	}
	else {
		UE_LOG(TCWLogErrors, Error, TEXT("Failed to get \"%s\" Row"));// , cardName.ToString());
		UE_LOG(TCWLogErrors, Error, TEXT("Note: Prebuilt Decks may not have been generated. Open the Main Menu to generate the decks."));
	}
	return FCardData();
}

int32 UDeckFunctionLibrary::GetRandomCardFromDeck(AController* controller, FName& cardName, ECardSet& cardset)
{
	//TODO: UDeckFunctionLibrary::GetRandomCardFromDeck
	return int32();
}

FName UDeckFunctionLibrary::GetCardFromDeck(TArray<FName> deck, int32 index, bool lastIndex)
{
	//TODO: UDeckFunctionLibrary::GetCardFromDeck
	return FName();
}

int32 UDeckFunctionLibrary::FindCardInArray(FName cardName, TArray<FName> cardArray, TArray<int32>& indexesArray)
{
	//TODO: UDeckFunctionLibrary::FindCardInArray
	return int32();
}

TArray<FName> UDeckFunctionLibrary::GetAllCardsInActiveSet(ECardSet cardset)
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