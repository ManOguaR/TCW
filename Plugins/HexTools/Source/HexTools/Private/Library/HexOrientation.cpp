// Copyrigth (c) 2020 SoFtwar 19.23 NGS. All rigths reserved. 


#include "HexOrientation.h"

const HexOrientation HexOrientation::LayoutPointy = HexOrientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const HexOrientation HexOrientation::LayoutFlat = HexOrientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

HexOrientation::HexOrientation(float F0, float F1, float F2, float F3, float B0, float B1, float B2, float B3, float startAngle) :
	F0(F0), F1(F1), F2(F2), F3(F3), B0(B0), B1(B1), B2(B2), B3(B3), StartAngle(startAngle)
{
}

HexOrientation::~HexOrientation()
{
}
