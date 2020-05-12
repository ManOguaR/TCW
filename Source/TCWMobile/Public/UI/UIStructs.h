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
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float RadialOffset;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float SpacingMin;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float SpacingMax;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		bool DynamicSpacing;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float ScreenOffsetX;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float ScreenOffsetY;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float RotationScaleFactor;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		int32 MaxCardsInHand;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		bool FlyInDirection;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		FVector2D TransformPivot;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float Scale;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		float Offset;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		bool RotateCardsToOffset;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Layout")
		EPositioningProfile PositioningProfile;
};
