// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct HEXTOOLS_API HexOrientation
{
public:
    static const HexOrientation LayoutPointy;
    static const HexOrientation LayoutFlat;

    const float F0;
    const float F1;
    const float F2;
    const float F3;
    const float B0;
    const float B1;
    const float B2;
    const float B3;
    const float StartAngle;
    HexOrientation(float f0, float f1, float f2, float f3, float b0, float b1, float b2, float b3, float startAngle);
    ~HexOrientation();
};
