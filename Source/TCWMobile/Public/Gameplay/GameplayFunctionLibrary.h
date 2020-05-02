// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TCWMOBILE_API UGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FRotator GetWorldRotationForPlayer(UObject* callerObject, FRotator overrideRotation, bool& cameraValid);
};
