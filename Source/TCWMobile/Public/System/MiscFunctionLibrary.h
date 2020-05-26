// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Slate/WidgetTransform.h"

#include "MiscFunctionLibrary.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TCWMOBILE_API UMiscFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UMiscFunctionLibrary(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get World"), Category = "Misc Tools")
		static UWorld* GetWorldReference();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Can Execute Cosmetic Events?"), Category = "Misc Tools")
		static bool CanExecuteCosmeticEvents(UObject* callerObject);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Interp To Goal Location 2D"), Category = "Graphics Math Tools")
		static FWidgetTransform InterpToGoalLocation2D(FWidgetTransform currentTransform, FWidgetTransform goalTransform, float deltaTime, float interpSpeed, bool& arrived);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Modify DPI Scaling"), Category = "Graphics Math Tools")
		static float ModifyDPIScaling(UObject* callerObject, float value, bool invertDPIScaling);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Mouse Position in Range"), Category = "Graphics Math Tools")
		static FVector2D GetMousePositionInRange(UObject* callerObject, FVector2D sizeOffset, float globalEdgeOffset, float XEdgeOffset, float YEdgeOffset);
	
};
