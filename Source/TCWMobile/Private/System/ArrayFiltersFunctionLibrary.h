// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Enums.h"
#include "SystemEnums.h"

#include "ArrayFiltersFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class UArrayFiltersFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Card Filters")
		static TArray<FName> FilterIncludedCards(TArray<FName> filterArray, bool filterRarity, ECardRarity rarity, bool filterColor, ECardColor color, bool filterCost, int32 cost, bool ownedOnly = true);

	UFUNCTION(BlueprintCallable, Category = "Card Validation")
		static bool ValidateCardThroughFilter(FName cardName, ECardSet cardSet, bool filterRarity, ECardRarity rarity, bool filterColor, ECardColor color, bool filterCost, int32 cost, bool ownedOnly = true);

	UFUNCTION(BlueprintCallable, Category = "Card Filters")
		static TArray<FName> SortCardArrayByName(TArray<FName> filterArray, EAlphabeticalSorting alphabeticalSort);

	UFUNCTION(BlueprintCallable, Category = "Card Filters")
		static TArray<FName> SortCardArrayByCost(TArray<FName> filterArray, ECostSorting costSort);

	UFUNCTION(BlueprintCallable, Category = "Card Filters")
		static TArray<int32> GetManaInDeck(TArray<FName> Array, float& average);
};
