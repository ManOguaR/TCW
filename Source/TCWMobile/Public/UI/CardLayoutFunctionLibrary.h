// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Slate/WidgetTransform.h"

#include "CardLayoutFunctionLibrary.generated.h"

class UCardWidget;

/**
 * 
 */
UCLASS()
class TCWMOBILE_API UCardLayoutFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UCardLayoutFunctionLibrary(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		static FWidgetTransform Fanned(UCardWidget* cardWidgetReference, int32 cardLoopIndex, int32 cardsInHand, int32 maxCardsInHand, FVector2D screenResolution, FVector2D cardSize, bool flyInDirection, bool rotateCardsToOffset, float cardOffset, float rotationDistanceScaleFactor, float radiusOffset, float cardSpacingMin, bool dynamicSpacing, float dynamicSpacingMaxOffset, float screenOffsetX, float screenOffsetY, float viewportScale, float cardScale);
	UFUNCTION(BlueprintCallable)
		static FWidgetTransform Horizontal(UCardWidget* cardWidgetReference, int32 cardLoopIndex, int32 cardsInHand, int32 maxCardsInHand, FVector2D screenResolution, FVector2D cardSize, bool flyInDirection, bool rotateCardsToOffset, float radiusOffset, float rotationDistanceScaleFactor, float cardSpacing, bool dynamicSpacing, float cardSpacingMax, float screenOffsetX, float screenOffsetY, float cardScale, float viewportScale, float cardOffset);

};
