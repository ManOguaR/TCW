// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Slate/WidgetTransform.h"

#include "MiscFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TCWMOBILE_API UMiscFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	static UWorld* GetWorldReference();
	static bool CanExecuteCosmeticEvents(UObject* callerObject);

	static FWidgetTransform InterpToGoalLocation2D(FWidgetTransform currentTransform, FWidgetTransform goalTransform, float deltaTime, float interpSpeed, bool& arrived);

	static float ModifyDPIScaling(UObject* callerObject, float value, bool invertDPIScaling);
};
