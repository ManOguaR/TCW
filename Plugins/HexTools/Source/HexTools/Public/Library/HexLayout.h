// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "HexOrientation.h"

/**
 *
 */
struct HEXTOOLS_API HexLayout
{
public:
	const HexOrientation Orientation;
	const FVector2D Size;
	const FVector2D Origin;

	HexLayout(HexOrientation orientation, FVector2D size, FVector2D origin) :
		Orientation(orientation), Size(size), Origin(origin) {}
	~HexLayout() {};
};
