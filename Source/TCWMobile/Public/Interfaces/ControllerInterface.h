// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "BoardUnit.h"

#include "ControllerInterface.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UControllerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API IControllerInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		ABoardUnit* CreatePlayableCard(FTransform spawnTransform);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool AddCardToPlayersHand(FName cardName);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginPlayerTurn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndPlayerTurn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUI();
};
