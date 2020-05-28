// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "Engine.h"

#include "UIEnums.h"

#include "UIStructs.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FCardLayout : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float RadialOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float SpacingMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float SpacingMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		bool DynamicSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float ScreenOffsetX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float ScreenOffsetY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float RotationScaleFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		int32 MaxCardsInHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		bool FlyInDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		FVector2D TransformPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		float Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		bool RotateCardsToOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Layout")
		EPositioningProfile PositioningProfile;
};
