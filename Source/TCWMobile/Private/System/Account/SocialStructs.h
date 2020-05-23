// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "SocialStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerFriend
{
	GENERATED_BODY()
	
public:
    FPlayerFriend() : 
        FPlayerFriend(TEXT("Empty"), -1) { };

	FPlayerFriend(FString playFabId, int32 index) :
        UserIndex(index),PlayFabId(playFabId)
        { };

    UPROPERTY(VisibleAnywhere, Category = "System")
        uint32 UserIndex;

    UPROPERTY(VisibleAnywhere, Category = "Profile")
        FString UserName;

private:
	FString PlayFabId;
};
