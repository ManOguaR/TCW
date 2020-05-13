// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "SystemEnums.h"

#include "AccountData.generated.h"

/**
 * 
 */
UCLASS()
class UAccountData : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleAnywhere, Category = "Internal Savegame")
    FString SaveSlotName;
    UPROPERTY(VisibleAnywhere, Category = "Internal Savegame")
    uint32 UserIndex;

    UPROPERTY(VisibleAnywhere, Category = "User Account")
    FString UserName;
    UPROPERTY(VisibleAnywhere, Category = "User Account")
    FString UserEmail;
    UPROPERTY(VisibleAnywhere, Category = "User Account")
    FString UserPassword;

    UPROPERTY(VisibleAnywhere, Category = "System")
    EAccountType AccountType;

    UAccountData(const FObjectInitializer& ObjectInitializer);
};
