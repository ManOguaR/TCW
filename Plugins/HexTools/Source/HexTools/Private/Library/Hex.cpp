// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "Hex.h"

Hex::Hex() :Hex(0, 0, 0)
{
}

Hex::Hex(int q, int r, int s) :
	Q(q), R(r), S(s)
{
	if (Q + R + S != 0)
		throw "Q + R + S must be 0";
}

Hex::~Hex()
{
}

Hex Hex::Add(Hex a, Hex b)
{
	return a.Add(b);
}

Hex Hex::Add(Hex other)
{
	return Hex(Q + other.Q, R + other.R, S + other.S);
}

Hex Hex::Subtract(Hex a, Hex b)
{
	return a.Subtract(b);
}

Hex Hex::Subtract(Hex other)
{
	return Hex(Q - other.Q, R - other.R, S - other.S);
}

Hex Hex::Scale(Hex a, int scale)
{
	return a.Scale(scale);
}

Hex Hex::Scale(int scale)
{
	return Hex(Q * scale, R * scale, S * scale);
}

Hex Hex::RotateLeft(Hex a)
{
	return a.RotateLeft();
}

Hex Hex::RotateLeft()
{
	return Hex(-S, -Q, -R);
}

Hex Hex::RotateRight(Hex a)
{
	return a.RotateRight();
}

Hex Hex::RotateRight()
{
	return Hex(-R, -S, -Q);
}

bool Hex::Equal(Hex a, Hex b)
{
	return (a.Q == b.Q && a.S == b.S && a.R == b.R);
}

bool Hex::Equal(Hex other)
{
	return (Q == other.Q && S == other.S && R == other.R);
}

Hex Hex::Neighbor(Hex hex, int direction)
{
	return hex.Neighbor(direction);
}

Hex Hex::Neighbor(int direction)
{
	return Add(Direction(direction));
}

Hex Hex::DiagonalNeighbor(Hex hex, int direction)
{
	return hex.DiagonalNeighbor(direction);
}

Hex Hex::DiagonalNeighbor(int direction)
{
	return Add(Diagonals[direction]);
}

const TArray<Hex> Hex::Directions = {
	Hex(1, 0, -1),
	Hex(1, -1, 0),
	Hex(0, -1, 1),
	Hex(-1, 0, 1),
	Hex(-1, 1, 0),
	Hex(0, 1, -1)
};
const TArray<Hex> Hex::Diagonals = {
	Hex(2, -1, -1),
	Hex(1, -2, 1),
	Hex(-1, -1, 2),
	Hex(-2, 1, 1),
	Hex(-1, 2, -1),
	Hex(1, 1, -2)
};

int Hex::Length(Hex hex)
{
	return hex.Length();
}

int Hex::Length()
{
	return int((abs(Q) + abs(R) + abs(S)) / 2);
}

int Hex::Distance(Hex a, Hex b)
{
	return a.Distance(b);
}

int Hex::Distance(Hex other)
{
	return Subtract(other).Length();
}

FractionalHex::FractionalHex(float q, float r, float s) :
	Q(q), R(r), S(s)
{
	if (round(Q + R + S) != 0)
		throw "Q + R + S must be 0";
}

FractionalHex::~FractionalHex()
{
}


Hex FractionalHex::Round(FractionalHex h)
{
	return h.Round();
}

Hex FractionalHex::Round()
{
	int qi = int(round(Q));
	int ri = int(round(R));
	int si = int(round(S));
	float q_diff = abs(qi - Q);
	float r_diff = abs(ri - R);
	float s_diff = abs(si - S);
	if (q_diff > r_diff && q_diff > s_diff)
	{
		qi = -ri - si;
	}
	else
		if (r_diff > s_diff)
		{
			ri = -qi - si;
		}
		else
		{
			si = -qi - ri;
		}
	return Hex(qi, ri, si);
}

FractionalHex FractionalHex::Lerp(FractionalHex a, FractionalHex b, float t)
{
	return a.Lerp(b, t);
}

FractionalHex FractionalHex::Lerp(FractionalHex other, float t)
{
	return FractionalHex(Q * (1.0 - t) + other.Q * t, R * (1.0 - t) + other.R * t, S * (1.0 - t) + other.S * t);
}
