// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "ArrayFiltersFunctionLibrary.h"
#include "DeckFunctionLibrary.h"

TArray<FName> UArrayFiltersFunctionLibrary::FilterIncludedCards(TArray<FName> filterArray, bool filterRarity, ECardRarity rarity, bool filterColor, ECardColor color, bool filterCost, int32 cost, bool ownedOnly)
{
	TArray<FName> Temp_IncludedCardArray = filterArray;
	TArray<FName> IncludedCardArray;

	for (FName each : Temp_IncludedCardArray)
	{
		if (ValidateCardThroughFilter(each, ECardSet::CardSet_Empty, filterRarity, rarity, filterColor, color, filterCost, cost, ownedOnly))
			IncludedCardArray.Add(each);
	}
	return IncludedCardArray;
}

bool UArrayFiltersFunctionLibrary::ValidateCardThroughFilter(FName cardName, ECardSet cardSet, bool filterRarity, ECardRarity rarity, bool filterColor, ECardColor color, bool filterCost, int32 cost, bool ownedOnly)
{
	FCardData card = UDeckFunctionLibrary::GetCardData(cardName, cardSet);

	return (!filterRarity || card.Rarity == rarity) && (!filterColor || card.Color == color) && (!filterCost || card.PlacementSettings.Cost == cost) && (!ownedOnly || card.DeckSettings.Unlocked);
}

TArray<FName> UArrayFiltersFunctionLibrary::SortCardArrayByName(TArray<FName> filterArray, EAlphabeticalSorting alphabeticalSort)
{
	TArray<FName> tempArray = filterArray;
	TArray<FName> IncludedCardArray;

	TArray<FString> alphabet = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
								 "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
								 "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
								 "U", "V", "W", "X", "Y", "Z" };

	for (int32 filterIndex = (alphabeticalSort == EAlphabeticalSorting::Sorting_AtoZ ? 0 : alphabet.Num());
		(alphabeticalSort == EAlphabeticalSorting::Sorting_AtoZ ? filterIndex < alphabet.Num() : filterIndex > 0);
		(alphabeticalSort == EAlphabeticalSorting::Sorting_AtoZ ? filterIndex++ : filterIndex--))
	{
		for (FName each : tempArray)
		{
			if (each.ToString().StartsWith(alphabet[filterIndex], ESearchCase::IgnoreCase))
			{
				IncludedCardArray.Add(each);
			}
		}
	}

	return IncludedCardArray;
}

TArray<FName> UArrayFiltersFunctionLibrary::SortCardArrayByCost(TArray<FName> filterArray, ECostSorting costSort)//, int32 costMin, int32 costMax)
{
	TArray<FName> tempArray = filterArray;
	TArray<FName> IncludedCardArray;

	for (int32 filterIndex = (costSort == ECostSorting::Sorting_LowToHigh ? 0 : 20);
		(costSort == ECostSorting::Sorting_LowToHigh ? filterIndex < 21 : filterIndex > 0);
		(costSort == ECostSorting::Sorting_LowToHigh ? filterIndex++ : filterIndex--))
	{
		for (FName each : tempArray)
		{
			FCardData card = UDeckFunctionLibrary::GetCardData(each, ECardSet::CardSet_Empty);

			if (card.PlacementSettings.Cost == filterIndex)
			{
				IncludedCardArray.Add(each);
			}
		}
	}

	return IncludedCardArray;
}
