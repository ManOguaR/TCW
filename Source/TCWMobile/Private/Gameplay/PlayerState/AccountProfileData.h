// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CardStructs.h"

#include "AccountProfileData.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class UAccountProfileData : public UObject
{
	GENERATED_BODY()

public:
	UAccountProfileData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetCredits, Category = "Player Currency")
		int32 Credits;
	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetShares, Category = "Player Currency")
		int32 Shares;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetNumEventTickets, Category = "Player Inventory")
		int32 NumEventTickets;
	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetNumCardPacks, Category = "Player Inventory")
		int32 NumCardPacks;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetCollection, Category = "Player Inventory")
		TMap<FName, int32> Collection;

private:
	UFUNCTION(BlueprintPure)
		const int32 GetCredits() { return playerCredits; };
	UFUNCTION(BlueprintPure)
		const int32 GetShares() { return playerShares; };

	UFUNCTION(BlueprintPure)
		int32 GetNumEventTickets() { return playerEventTickets; };
	UFUNCTION(BlueprintPure)
		int32 GetNumCardPacks();

	UFUNCTION(BlueprintPure)
		TMap<FName, int32> GetCollection() { return playerCardCollection; };

	int32 playerCredits;
	int32 playerShares;
	int32 playerEventTickets;

	TMap<FName, int32> playerCardPacks;
	TMap<FName, int32> playerCardCollection;
};
