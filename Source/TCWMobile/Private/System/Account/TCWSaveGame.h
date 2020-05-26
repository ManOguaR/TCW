// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TCWSaveGame.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class UTCWSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetDeckList, Category = "Deck SaveGame")
		TArray<FString> DeckList;

	UTCWSaveGame(const FObjectInitializer& ObjectInitializer);

private:
	UFUNCTION(BlueprintPure)
		TArray<FString> GetDeckList();

};
