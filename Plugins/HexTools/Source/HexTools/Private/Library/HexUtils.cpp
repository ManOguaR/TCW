// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "HexUtils.h"

TArray<Hex> HexUtils::LineDraw(Hex origin, Hex end)
{
	int N = Hex::Distance(origin, end);
	FractionalHex origin_nudge = FractionalHex(origin.Q + 1e-06, origin.R + 1e-06, origin.S - 2e-06);
	FractionalHex end_nudge = FractionalHex(end.Q + 1e-06, end.R + 1e-06, end.S - 2e-06);
	TArray<Hex> results = {};
	float step = 1.0 / FMath::Max(N, 1);
	for (int i = 0; i <= N; i++)
	{
		results.Add(FractionalHex::Round(FractionalHex::Lerp(origin_nudge, end_nudge, step * i)));
	}
	return results;
}

OffsetCoord HexUtils::QOffsetFromCube(int offset, Hex h)
{
	int col = h.Q;
	int row = h.R + int((h.Q + offset * (h.Q & 1)) / 2);
	if (offset != EVEN && offset != ODD)
	{
		throw "offset must be EVEN (+1) or ODD (-1)";
	}
	return OffsetCoord(col, row);
}

Hex HexUtils::QOffsetToCube(int offset, OffsetCoord h)
{
	int q = h.Col;
	int r = h.Row - int((h.Col + offset * (h.Col & 1)) / 2);
	int s = -q - r;
	if (offset != EVEN && offset != ODD)
	{
		throw "offset must be EVEN (+1) or ODD (-1)";
	}
	return Hex(q, r, s);
}

OffsetCoord HexUtils::ROffsetFromCube(int offset, Hex h)
{
	int col = h.Q + int((h.R + offset * (h.R & 1)) / 2);
	int row = h.R;
	if (offset != EVEN && offset != ODD)
	{
		throw "offset must be EVEN (+1) or ODD (-1)";
	}
	return OffsetCoord(col, row);
}

Hex HexUtils::ROffsetToCube(int offset, OffsetCoord h)
{
	int q = h.Col - int((h.Row + offset * (h.Row & 1)) / 2);
	int r = h.Row;
	int s = -q - r;
	if (offset != EVEN && offset != ODD)
	{
		throw "offset must be EVEN (+1) or ODD (-1)";
	}
	return Hex(q, r, s);
}

DoubledCoord HexUtils::QDoubledFromCube(Hex h)
{
	int col = h.Q;
	int row = 2 * h.R + h.Q;
	return DoubledCoord(col, row);
}

Hex HexUtils::QDoubledToCube(DoubledCoord h)
{
	int q = h.Col;
	int r = int((h.Row - h.Col) / 2);
	int s = -q - r;
	return Hex(q, r, s);
}

DoubledCoord HexUtils::RDoubledFromCube(Hex h)
{
	int col = 2 * h.Q + h.R;
	int row = h.R;
	return DoubledCoord(col, row);
}

Hex HexUtils::RDoubledToCube(DoubledCoord h)
{
	int q = int((h.Col - h.Row) / 2);
	int r = h.Row;
	int s = -q - r;
	return Hex(q, r, s);
}

FVector2D HexUtils::HexToPixel(HexLayout layout, Hex h)
{
	HexOrientation M = layout.Orientation;
	FVector2D size = layout.Size;
	FVector2D origin = layout.Origin;
	float x = (M.F0 * h.Q + M.F1 * h.R) * size.X;
	float y = (M.F2 * h.Q + M.F3 * h.R) * size.Y;
	return FVector2D(x + origin.X, y + origin.Y);
}

FractionalHex HexUtils::PixelToHex(HexLayout layout, FVector2D p)
{
	HexOrientation M = layout.Orientation;
	FVector2D size = layout.Size;
	FVector2D origin = layout.Origin;
	FVector2D pt = FVector2D((p.X - origin.X) / size.X, (p.Y - origin.Y) / size.Y);
	float q = M.B0 * pt.X + M.B1 * pt.Y;
	float r = M.B2 * pt.X + M.B3 * pt.Y;
	return FractionalHex(q, r, -q - r);
}

FVector2D HexUtils::CornerOffset(HexLayout layout, int corner)
{
	HexOrientation M = layout.Orientation;
	FVector2D size = layout.Size;
	float angle = 2.0 * PI * (M.StartAngle - corner) / 6.0;
	return FVector2D(size.X * cos(angle), size.Y * sin(angle));
}

TArray<FVector2D> HexUtils::PolygonCorners(HexLayout layout, Hex h)
{
	TArray<FVector2D> corners = {};
	FVector2D center = HexToPixel(layout, h);
	for (int i = 0; i < 6; i++)
	{
		FVector2D offset = CornerOffset(layout, i);
		corners.Add(FVector2D(center.X + offset.X, center.Y + offset.Y));
	}
	return corners;
}
