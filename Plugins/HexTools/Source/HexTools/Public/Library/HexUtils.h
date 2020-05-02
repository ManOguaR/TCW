// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Hex.h"
#include "HexCoords.h"
#include "HexLayout.h"

/**
 * 
 */
class HEXTOOLS_API HexUtils
{
public:
    static const int EVEN = 1;
    static const int ODD = -1;

public:
    static TArray<Hex> LineDraw(Hex origin, Hex end);

    static OffsetCoord QOffsetFromCube(int offset, Hex h);
    static Hex QOffsetToCube(int offset, OffsetCoord h);

    static OffsetCoord ROffsetFromCube(int offset, Hex h);
    static Hex ROffsetToCube(int offset, OffsetCoord h);

    static DoubledCoord QDoubledFromCube(Hex h);
    static Hex QDoubledToCube(DoubledCoord h);

    static DoubledCoord RDoubledFromCube(Hex h);
    static Hex RDoubledToCube(DoubledCoord h);

    static FVector2D HexToPixel(HexLayout layout, Hex h);
    static FractionalHex PixelToHex(HexLayout layout, FVector2D p);

    static FVector2D CornerOffset(HexLayout layout, int corner);
    static TArray<FVector2D> PolygonCorners(HexLayout layout, Hex h);
};

