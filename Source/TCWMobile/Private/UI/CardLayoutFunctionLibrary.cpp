// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "CardLayoutFunctionLibrary.h"

UCardLayoutFunctionLibrary::UCardLayoutFunctionLibrary(const FObjectInitializer& ObjectInitializer) : UBlueprintFunctionLibrary(ObjectInitializer)
{

}

FWidgetTransform UCardLayoutFunctionLibrary::Fanned(UCardWidget* cardWidgetReference, int32 cardLoopIndex, int32 cardsInHand, int32 maxCardsInHand, FVector2D screenResolution, FVector2D cardSize, bool flyInDirection, bool rotateCardsToOffset, float cardOffset, float rotationDistanceScaleFactor, float radiusOffset, float cardSpacingMin, bool dynamicSpacing, float dynamicSpacingMaxOffset, float screenOffsetX, float screenOffsetY, float viewportScale, float cardScale)
{
	//TDOD: CHECK MATHS
	return FWidgetTransform(
		FVector2D((screenOffsetX + ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (dynamicSpacing ? FMath::Clamp(FMath::Lerp(dynamicSpacingMaxOffset, cardSpacingMin, ((float)cardsInHand / (float)maxCardsInHand)), cardSpacingMin, dynamicSpacingMaxOffset) : cardSpacingMin)) * (flyInDirection ? 1.0f : -1.0f) * cardScale)), ((((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (dynamicSpacing ? FMath::Clamp(FMath::Lerp(dynamicSpacingMaxOffset, cardSpacingMin, ((float)cardsInHand / (float)maxCardsInHand)), cardSpacingMin, dynamicSpacingMaxOffset) : cardSpacingMin)) * radiusOffset)) * -1.0f) - screenOffsetY) * cardScale)),
		FVector2D(cardScale),
		FVector2D(0.0f),
		(rotateCardsToOffset ? ((float)cardLoopIndex >= (((float)(cardsInHand - 1) / 2.0f) + cardOffset) ? ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (rotationDistanceScaleFactor * 20.0f)) * (flyInDirection ? -1.0f : 1.0f)) : ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (rotationDistanceScaleFactor * 20.0f)) * (flyInDirection ? -1.0f : 1.0f))) : 0.0f) * -1.0f);
}

FWidgetTransform UCardLayoutFunctionLibrary::Horizontal(UCardWidget* cardWidgetReference, int32 cardLoopIndex, int32 cardsInHand, int32 maxCardsInHand, FVector2D screenResolution, FVector2D cardSize, bool flyInDirection, bool rotateCardsToOffset, float radiusOffset, float rotationDistanceScaleFactor, float cardSpacing, bool dynamicSpacing, float cardSpacingMax, float screenOffsetX, float screenOffsetY, float cardScale, float viewportScale, float cardOffset)
{
	//TDOD: CHECK MATHS
	return FWidgetTransform(
		FVector2D(),
		//	(screenOffsetX + ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (dynamicSpacing ? FMath::Clamp(FMath::Lerp(dynamicSpacingMaxOffset, cardSpacingMin, ((float)cardsInHand / (float)maxCardsInHand)), cardSpacingMin, dynamicSpacingMaxOffset) : cardSpacingMin)) * (flyInDirection ? 1.0f : -1.0f) * cardScale)),
		//	(
		//		(
		//			(
		//				(
		//					(((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (dynamicSpacing ? FMath::Clamp(FMath::Lerp(dynamicSpacingMaxOffset, cardSpacingMin, ((float)cardsInHand / (float)maxCardsInHand)), cardSpacingMin, dynamicSpacingMaxOffset) : cardSpacingMin)) * radiusOffset)) 
		//					* -1.0f)
		//				+ 1.0f) 
		//			- screenOffsetY)
		//		* cardScale
		//		)
		//),
		FVector2D(cardScale),
		FVector2D(0.0f),
		(rotateCardsToOffset ? ((float)cardLoopIndex >= (((float)(cardsInHand - 1) / 2.0f) + cardOffset) ? ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (rotationDistanceScaleFactor * 20.0f)) * (flyInDirection ? -1.0f : 1.0f)) : ((((float)cardLoopIndex - (((float)(cardsInHand - 1) / 2.0f) + cardOffset)) * (rotationDistanceScaleFactor * 20.0f)) * (flyInDirection ? -1.0f : 1.0f))) : 0.0f) * -1.0f);
}
