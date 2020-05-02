// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveGameFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TCWMOBILE_API USaveGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		static void RemoveSaveGameSlot(FString saveGame);
	UFUNCTION(BlueprintCallable)
		static bool CreateLoadCardGameSave(FString slotName, TSubclassOf<USaveGame> saveGameClass, USaveGame* saveGame);
	UFUNCTION(BlueprintCallable)
		static bool SaveCustomDeck(FText& inText, TArray<FName> customDeck, bool editable);
	UFUNCTION(BlueprintCallable)
		static TArray<FName> LoadCustomDeck(FString deckName, bool& deckEditable, bool& deckValid);
	UFUNCTION(BlueprintCallable)
		static FString GetRandomDeck(bool& deckValid);
	UFUNCTION(BlueprintPure)
		static bool GetDeckValid(FString slotName);
	UFUNCTION(BlueprintPure)
		static bool GetDeckEditable(FString slotName);

};
