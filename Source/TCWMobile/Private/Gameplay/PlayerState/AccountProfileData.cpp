// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "AccountProfileData.h"

UAccountProfileData::UAccountProfileData(const FObjectInitializer& ObjectInitializer)
{
	//PRE - 02
}

int32 UAccountProfileData::GetNumCardPacks()
{
	int32 result = 0;
	for (auto& elem : playerCardPacks)
	{
		result += elem.Value;
	}
	return result;
}
