// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "BoardUnit.h"

#include "BoardState.generated.h"

UCLASS()
class TCWMOBILE_API UBoardState : public UObject
{
	GENERATED_BODY()

public:
	UBoardState();

public:
	TArray<ABoardUnit*> PlayerUnits;
	TArray<ABoardUnit*> OpponentUnits;

};
