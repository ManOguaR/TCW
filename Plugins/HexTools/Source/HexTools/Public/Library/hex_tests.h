// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Hex.h"
#include "HexCoords.h"
#include "HexLayout.h"
#include "HexUtils.h"

// Forward declarations

// Tests
#include <iostream>

void complain(const char* name)
{
	std::cout << "FAIL " << name << std::endl;
}

void equal_hex(const char* name, Hex a, Hex b)
{
	if (!(a.Q == b.Q && a.S == b.S && a.R == b.R))
	{
		complain(name);
	}
}

void equal_offsetcoord(const char* name, OffsetCoord a, OffsetCoord b)
{
	if (!(a.Col == b.Col && a.Row == b.Row))
	{
		complain(name);
	}
}

void equal_doubledcoord(const char* name, DoubledCoord a, DoubledCoord b)
{
	if (!(a.Col == b.Col && a.Row == b.Row))
	{
		complain(name);
	}
}

void equal_int(const char* name, int a, int b)
{
	if (!(a == b))
	{
		complain(name);
	}
}

void equal_hex_array(const char* name, TArray<Hex> a, TArray<Hex> b)
{
	equal_int(name, a.Num(), b.Num());
	for (int i = 0; i < a.Num(); i++)
	{
		equal_hex(name, a[i], b[i]);
	}
}



void test_hex_arithmetic()
{
	equal_hex("hex_add", Hex(4, -10, 6), Hex::Add(Hex(1, -3, 2), Hex(3, -7, 4)));
	equal_hex("hex_subtract", Hex(-2, 4, -2), Hex::Subtract(Hex(1, -3, 2), Hex(3, -7, 4)));
}

void test_hex_direction()
{
	equal_hex("hex_direction", Hex(0, -1, 1), Hex::Direction(2));
}

void test_hex_neighbor()
{
	equal_hex("hex_neighbor", Hex(1, -3, 2), Hex::Neighbor(Hex(1, -2, 1), 2));
}

void test_hex_diagonal()
{
	equal_hex("hex_diagonal", Hex(-1, -1, 2), Hex::DiagonalNeighbor(Hex(1, -2, 1), 3));
}

void test_hex_distance()
{
	equal_int("hex_distance", 7, Hex::Distance(Hex(3, -7, 4), Hex(0, 0, 0)));
}

void test_hex_rotate_right()
{
	equal_hex("hex_rotate_right", Hex::RotateRight(Hex(1, -3, 2)), Hex(3, -2, -1));
}

void test_hex_rotate_left()
{
	equal_hex("hex_rotate_left", Hex::RotateLeft(Hex(1, -3, 2)), Hex(-2, -1, 3));
}

void test_hex_round()
{
	FractionalHex a = FractionalHex(0.0, 0.0, 0.0);
	FractionalHex b = FractionalHex(1.0, -1.0, 0.0);
	FractionalHex c = FractionalHex(0.0, -1.0, 1.0);
	equal_hex("hex_round 1", Hex(5, -10, 5), FractionalHex::Round(FractionalHex::Lerp(FractionalHex(0.0, 0.0, 0.0), FractionalHex(10.0, -20.0, 10.0), 0.5)));
	equal_hex("hex_round 2", FractionalHex::Round(a), FractionalHex::Round(FractionalHex::Lerp(a, b, 0.499)));
	equal_hex("hex_round 3", FractionalHex::Round(b), FractionalHex::Round(FractionalHex::Lerp(a, b, 0.501)));
	equal_hex("hex_round 4", FractionalHex::Round(a), FractionalHex::Round(FractionalHex(a.Q * 0.4 + b.Q * 0.3 + c.Q * 0.3, a.R * 0.4 + b.R * 0.3 + c.R * 0.3, a.S * 0.4 + b.S * 0.3 + c.S * 0.3)));
	equal_hex("hex_round 5", FractionalHex::Round(c), FractionalHex::Round(FractionalHex(a.Q * 0.3 + b.Q * 0.3 + c.Q * 0.4, a.R * 0.3 + b.R * 0.3 + c.R * 0.4, a.S * 0.3 + b.S * 0.3 + c.S * 0.4)));
}

void test_hex_linedraw()
{
	equal_hex_array("hex_linedraw", { Hex(0, 0, 0), Hex(0, -1, 1), Hex(0, -2, 2), Hex(1, -3, 2), Hex(1, -4, 3), Hex(1, -5, 4) }, HexUtils::LineDraw(Hex(0, 0, 0), Hex(1, -5, 4)));
}

void test_layout()
{
	Hex h = Hex(3, 4, -7);
	HexLayout flat = HexLayout(HexOrientation::LayoutFlat, FVector2D(10.0, 15.0), FVector2D(35.0, 71.0));
	equal_hex("layout", h, FractionalHex::Round(HexUtils::PixelToHex(flat, HexUtils::HexToPixel(flat, h))));
	HexLayout pointy = HexLayout(HexOrientation::LayoutPointy, FVector2D(10.0, 15.0), FVector2D(35.0, 71.0));
	equal_hex("layout", h, FractionalHex::Round(HexUtils::PixelToHex(pointy, HexUtils::HexToPixel(pointy, h))));
}

void test_offset_roundtrip()
{
	Hex a = Hex(3, 4, -7);
	OffsetCoord b = OffsetCoord(1, -3);
	equal_hex("conversion_roundtrip even-q", a, HexUtils::QOffsetToCube(HexUtils::EVEN, HexUtils::QOffsetFromCube(HexUtils::EVEN, a)));
	equal_offsetcoord("conversion_roundtrip even-q", b, HexUtils::QOffsetFromCube(HexUtils::EVEN, HexUtils::QOffsetToCube(HexUtils::EVEN, b)));
	equal_hex("conversion_roundtrip odd-q", a, HexUtils::QOffsetToCube(HexUtils::ODD, HexUtils::QOffsetFromCube(HexUtils::ODD, a)));
	equal_offsetcoord("conversion_roundtrip odd-q", b, HexUtils::QOffsetFromCube(HexUtils::ODD, HexUtils::QOffsetToCube(HexUtils::ODD, b)));
	equal_hex("conversion_roundtrip even-r", a, HexUtils::ROffsetToCube(HexUtils::EVEN, HexUtils::ROffsetFromCube(HexUtils::EVEN, a)));
	equal_offsetcoord("conversion_roundtrip even-r", b, HexUtils::ROffsetFromCube(HexUtils::EVEN, HexUtils::ROffsetToCube(HexUtils::EVEN, b)));
	equal_hex("conversion_roundtrip odd-r", a, HexUtils::ROffsetToCube(HexUtils::ODD, HexUtils::ROffsetFromCube(HexUtils::ODD, a)));
	equal_offsetcoord("conversion_roundtrip odd-r", b, HexUtils::ROffsetFromCube(HexUtils::ODD, HexUtils::ROffsetToCube(HexUtils::ODD, b)));
}

void test_offset_from_cube()
{
	equal_offsetcoord("offset_from_cube even-q", OffsetCoord(1, 3), HexUtils::QOffsetFromCube(HexUtils::EVEN, Hex(1, 2, -3)));
	equal_offsetcoord("offset_from_cube odd-q", OffsetCoord(1, 2), HexUtils::QOffsetFromCube(HexUtils::ODD, Hex(1, 2, -3)));
}

void test_offset_to_cube()
{
	equal_hex("offset_to_cube even-", Hex(1, 2, -3), HexUtils::QOffsetToCube(HexUtils::EVEN, OffsetCoord(1, 3)));
	equal_hex("offset_to_cube odd-q", Hex(1, 2, -3), HexUtils::QOffsetToCube(HexUtils::ODD, OffsetCoord(1, 2)));
}

void test_doubled_roundtrip()
{
	Hex a = Hex(3, 4, -7);
	DoubledCoord b = DoubledCoord(1, -3);
	equal_hex("conversion_roundtrip doubled-q", a, HexUtils::QDoubledToCube(HexUtils::QDoubledFromCube(a)));
	equal_doubledcoord("conversion_roundtrip doubled-q", b, HexUtils::QDoubledFromCube(HexUtils::QDoubledToCube(b)));
	equal_hex("conversion_roundtrip doubled-r", a, HexUtils::RDoubledToCube(HexUtils::RDoubledFromCube(a)));
	equal_doubledcoord("conversion_roundtrip doubled-r", b, HexUtils::RDoubledFromCube(HexUtils::RDoubledToCube(b)));
}

void test_doubled_from_cube()
{
	equal_doubledcoord("doubled_from_cube doubled-q", DoubledCoord(1, 5), HexUtils::QDoubledFromCube(Hex(1, 2, -3)));
	equal_doubledcoord("doubled_from_cube doubled-r", DoubledCoord(4, 2), HexUtils::RDoubledFromCube(Hex(1, 2, -3)));
}

void test_doubled_to_cube()
{
	equal_hex("doubled_to_cube doubled-q", Hex(1, 2, -3), HexUtils::QDoubledToCube(DoubledCoord(1, 5)));
	equal_hex("doubled_to_cube doubled-r", Hex(1, 2, -3), HexUtils::RDoubledToCube(DoubledCoord(4, 2)));
}

void test_all()
{
	test_hex_arithmetic();
	test_hex_direction();
	test_hex_neighbor();
	test_hex_diagonal();
	test_hex_distance();
	test_hex_rotate_right();
	test_hex_rotate_left();
	test_hex_round();
	test_hex_linedraw();
	test_layout();
	test_offset_roundtrip();
	test_offset_from_cube();
	test_offset_to_cube();
	test_doubled_roundtrip();
	test_doubled_from_cube();
	test_doubled_to_cube();
}
