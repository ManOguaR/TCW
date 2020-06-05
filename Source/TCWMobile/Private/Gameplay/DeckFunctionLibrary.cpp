// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DeckFunctionLibrary.h"
#include "..\TCWMobile.h"

#include "Engine\DataTable.h"

TArray<FName> UDeckFunctionLibrary::GetChosenDeckArray(int32 index, TArray<UDataTable*> targetArray)
{
	return targetArray[index]->GetRowNames();
}

FCardData UDeckFunctionLibrary::GetCardData(FName cardName, ECardSet cardset)
{
	FCardData* result = nullptr;

	bool allSets = (cardset == ECardSet::CardSet_Empty);
	bool found = false;

	if (allSets || cardset == ECardSet::CardSet_Basic)
	{
		if (UDataTable* dataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Gameplay/BasicSet_DataTable.BasicSet_DataTable")))
		{
			result = dataTable->FindRow<FCardData>(cardName, TEXT("LookUp Operation"), !allSets);
			if (result) 
				found = true;
		}
		else
		{
			UE_LOG(TCWLogErrors, Fatal, TEXT("Failed to get DataTable"));
		}
	}
	if ((allSets && !found) || cardset == ECardSet::CardSet_Debug)
	{
		if (UDataTable* dataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Gameplay/DebugSet_DataTable.DebugSet_DataTable")))
		{
			result = dataTable->FindRow<FCardData>(cardName, TEXT("LookUp Operation"));
		}
		else
		{
			UE_LOG(TCWLogErrors, Fatal, TEXT("Failed to get DataTable"));
		}
	}

	if (result) {
		return FCardData(*result);
	}
	else {
		UE_LOG(TCWLogErrors, Error, TEXT("Failed to get \"%s\" Row"));// , cardName.ToString());
		UE_LOG(TCWLogErrors, Error, TEXT("Note: Prebuilt Decks may not have been generated. Open the Main Menu to generate the decks."));
	}
	return FCardData();
}

FName UDeckFunctionLibrary::GetRandomCardFromDeck(AController* controller, int32& index, ECardSet& cardset)
{
	cardset = ECardSet::CardSet_Empty;

	if (IDeckInterface* intfce = Cast<IDeckInterface>(controller))
	{
		TArray<FName> playerDeck = intfce->GetPlayerDeck();
		index = FMath::RandRange(0, playerDeck.Num() - 1);
		return playerDeck[index];
	}

	return FName();
}

FName UDeckFunctionLibrary::GetCardFromDeck(TArray<FName> deck, int32 index, bool lastIndex)
{
	return lastIndex ? deck.Last(0) : deck[index];
}

TArray<int32> UDeckFunctionLibrary::FindCardInArray(FName cardName, TArray<FName> cardArray, int32& found)
{
	TArray<int32> result = TArray<int32>();
	for (int32 ix = 0; ix < cardArray.Num(); ix++)
	{
		if (cardArray[ix] == cardName)
		{
			result.Add(ix);
		}
	}
	found = result.Num();
	return result;
}

TArray<FName> UDeckFunctionLibrary::GetAllCardsInActiveSet(ECardSet cardset)
{
	bool allCardSets = (cardset == ECardSet::CardSet_Empty);

	TArray<FName> result = TArray<FName>();

	if (allCardSets || (cardset == ECardSet::CardSet_Basic))
	{
		//ConstructorHelpers::FObjectFinder<UDataTable> Table_BP(TEXT("DataTable'/Game/Data/Gameplay/BasicSet_DataTable.BasicSet_DataTable'"));
		if (UDataTable* dataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Gameplay/BasicSet_DataTable.BasicSet_DataTable")))
		{
			//if (Table_BP.Succeeded())
			//{
				//UDataTable* dataTable = Table_BP.Object;

			TArray<FName> rowNames = dataTable->GetRowNames();
			for (FName rowName : rowNames)
			{
				result.Add(rowName);
			}
			//}
		}
	}

	if (allCardSets || (cardset == ECardSet::CardSet_Debug))
	{
		//ConstructorHelpers::FObjectFinder<UDataTable> Table_BP(TEXT("DataTable'/Game/Data/Gameplay/DebugSet_DataTable.DebugSet_DataTable'"));

		if (UDataTable* dataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Gameplay/DebugSet_DataTable.DebugSet_DataTable")))
		{
			//if (Table_BP.Succeeded())
			//{
			//	UDataTable* dataTable = Table_BP.Object;

			TArray<FName> rowNames = dataTable->GetRowNames();
			for (FName rowName : rowNames)
			{
				result.Add(rowName);
			}
			//}
		}
	}

	return result;
}

void UDeckFunctionLibrary::RemoveCardFromDeck(bool removeAll, int32 index, TArray<FName> deck)
{
	if (removeAll)
	{
		deck.Empty();
	}
	else
	{
		deck.RemoveAt(index);
	}
}

int32 UDeckFunctionLibrary::CountCardsInDeck(TArray<FName> deck)
{
	return deck.Num();
}

TArray<FName> UDeckFunctionLibrary::FilterWeightedCardsInDeck(TArray<FName> cardsInDeck)
{
	TArray<FName> localDeck = TArray<FName>();
	TArray<FName> tempDeck = cardsInDeck;

	localDeck.SetNum(tempDeck.Num());
	int32 ix = 0;
	for (FName each : tempDeck)
	{
		FCardData data = GetCardData(each, ECardSet::CardSet_Empty);

		//if ()
		//{

		//}

		ix++;
	}

	return localDeck;
}