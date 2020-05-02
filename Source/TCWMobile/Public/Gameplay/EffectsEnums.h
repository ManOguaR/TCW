// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBoardPlayerEffects : uint8
{
	PlayerEffects_DecreasedHealth		UMETA(DisplayName = "Decreased Health"),
	PlayerEffects_IncreasedHealth		UMETA(DisplayName = "Increased Health"),
	PlayerEffects_Death					UMETA(DisplayName = "Death")
};

UENUM(BlueprintType)
enum class ECardEffects : uint8
{
	CardEffects_OnCardPlaced		UMETA(DisplayName = "On Card Placed"),
	CardEffects_OnAttack			UMETA(DisplayName = "On Attack"),
	CardEffects_DecreasedHealth		UMETA(DisplayName = "Decreased Health"),
	CardEffects_IncreasedHealth		UMETA(DisplayName = "Increased Health"),
	CardEffects_OnAbilityUsed		UMETA(DisplayName = "On Ability Used"),
	CardEffects_OnDeath				UMETA(DisplayName = "On Death")
};
