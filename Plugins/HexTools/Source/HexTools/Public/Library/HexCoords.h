// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct HEXTOOLS_API OffsetCoord
{
public:
    const int Col;
    const int Row;
    OffsetCoord(int col, int row);
    ~OffsetCoord();
};

/**
 *
 */
struct HEXTOOLS_API DoubledCoord
{
    const int Col;
    const int Row;
    DoubledCoord(int col, int row);
    ~DoubledCoord();
};
