#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EViewState : uint8
{
	ViewState_Default			UMETA(DisplayName = "Default"),
	ViewState_CardInPreview		UMETA(DisplayName = "Card In Preview"),
	ViewState_ViewHand			UMETA(DisplayName = "View Hand"),
	ViewState_HideHand			UMETA(DisplayName = "Hide Hand"),
	ViewState_TestProfile		UMETA(DisplayName = "Test Profile")
};

UENUM(BlueprintType)
enum class EPositioningProfile : uint8
{
	PositioningProfile_Fanned			UMETA(DisplayName = "Fanned"),
	PositioningProfile_Horizontal		UMETA(DisplayName = "Horizontal")
};

