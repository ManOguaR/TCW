// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomDeckSave.generated.h"

/**
 * 
 */
UCLASS()
class UCustomDeckSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Deck SaveGame")
		TArray<FName> CustomDeck;
	UPROPERTY(VisibleAnywhere, Category = "Deck SaveGame")
		bool bEditable;

	UCustomDeckSave(const FObjectInitializer& ObjectInitializer);
};
