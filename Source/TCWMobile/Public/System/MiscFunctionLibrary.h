// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "MiscFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TCWMOBILE_API UMiscFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	//static void Delay(AActor* callerActor, float time);
	//static void Delay(UObject* callerObject, float time);
	static UWorld* GetWorldReference();
	static bool CanExecuteCosmeticEvents(UObject* callerObject);
};
