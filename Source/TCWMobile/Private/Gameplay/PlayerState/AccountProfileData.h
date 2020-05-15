// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Account\SocialStructs.h"
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

	UPROPERTY(BlueprintGetter = GetCredits, Category = "Player Currency")
		int32 Credits;
	UPROPERTY(BlueprintGetter = GetShares, Category = "Player Currency")
		int32 Shares;

	UPROPERTY(BlueprintGetter = GetNumEventTickets, Category = "Player Inventory")
		int32 NumEventTickets;
	UPROPERTY(BlueprintGetter = GetNumCardPacks, Category = "Player Inventory")
		int32 NumCardPacks;

	UPROPERTY(BlueprintGetter = GetCollection, Category = "Player Inventory")
		TMap<FName, int32> Collection;

public:
	UFUNCTION(BlueprintPure)
		TArray<FPlayerFriend> GetPlayerFriends();

	void AddPlayerFriend(FString playFabId, FString userName, int32 userIndex);

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

	TMap<ECardSet, int32> playerCardPacks;
	TMap<FName, int32> playerCardCollection;

	TArray<FPlayerFriend> playerFriends;
};
