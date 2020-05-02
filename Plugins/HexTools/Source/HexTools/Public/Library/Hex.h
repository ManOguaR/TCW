// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
struct HEXTOOLS_API Hex
{
public:
	const int Q;
	const int R;
	const int S;
	Hex();
	Hex(int q, int r, int s);
	~Hex();

public:
	static Hex Add(Hex a, Hex b);
	Hex Add(Hex other);
	static Hex Subtract(Hex a, Hex b);
	Hex Subtract(Hex other);
	static Hex Scale(Hex a, int scale);
	Hex Scale(int scale);
	static Hex RotateLeft(Hex a);
	Hex RotateLeft();
	static Hex RotateRight(Hex a);
	Hex RotateRight();

	static bool Equal(Hex a, Hex b);
	bool Equal(Hex other);

public:
	static Hex Neighbor(Hex hex, int direction);
	Hex Neighbor(int direction);
	static Hex DiagonalNeighbor(Hex hex, int direction);
	Hex DiagonalNeighbor(int direction);

private:
	static const TArray<Hex> Directions;
	static const TArray<Hex> Diagonals;
public:
	static Hex Direction(int direction)
	{
		return Directions[direction];
	}

	static int Length(Hex hex);
	int Length();

	static int Distance(Hex a, Hex b);
	int Distance(Hex other);
};

static bool operator == (Hex a, Hex b) {
	return a.Q == b.Q && a.R == b.R && a.S == b.S;
}

static bool operator != (Hex a, Hex b) {
	return !(a == b);
}
/**
 *
 */
struct HEXTOOLS_API FractionalHex
{
public:
	const float Q;
	const float R;
	const float S;
	FractionalHex(float q, float r, float s);
	~FractionalHex();
public:
	static Hex Round(FractionalHex h);
	Hex Round();

	static FractionalHex Lerp(FractionalHex a, FractionalHex b, float t);
	FractionalHex Lerp(FractionalHex other, float t);
};

