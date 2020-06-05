// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CardStructs.h"
#include "DeckInterface.h"

#include "DeckFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TCWMOBILE_API UDeckFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static TArray<FName> GetChosenDeckArray(int32 index, TArray<UDataTable*> targetArray);
	UFUNCTION(BlueprintCallable)
		static FCardData GetCardData(FName cardName, ECardSet cardset);
	UFUNCTION(BlueprintCallable)
		static FName GetRandomCardFromDeck(AController* controller, int32& cardName, ECardSet& cardset);
	UFUNCTION(BlueprintCallable)
		static FName GetCardFromDeck(TArray<FName> deck, int32 index, bool lastIndex);
	UFUNCTION(BlueprintCallable)
		static TArray<int32> FindCardInArray(FName cardName, TArray<FName> cardArray, int32& found);
	UFUNCTION(BlueprintCallable)
		static TArray<FName> GetAllCardsInActiveSet(ECardSet cardset);
	UFUNCTION(BlueprintCallable)
		static void RemoveCardFromDeck(bool removeAll, int32 index, TArray<FName> deck);
	UFUNCTION(BlueprintCallable)
		static int32 CountCardsInDeck(TArray<FName> deck);
	UFUNCTION(BlueprintCallable)
		static TArray<FName> FilterWeightedCardsInDeck(TArray<FName> cardsInDeck);


};
