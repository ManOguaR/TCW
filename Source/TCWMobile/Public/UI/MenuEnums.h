// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EArenaList : uint8
{
	Arena_Empty				UMETA(DisplayName = "Empty"),
	Arena_Classic			UMETA(DisplayName = "Arena_Classic"),
	Arena_Doubles			UMETA(DisplayName = "Arena_Doubles"),
	Arena_EaglesNest		UMETA(DisplayName = "Arena_EaglesNest"),
	Arena_FlatGrounds		UMETA(DisplayName = "Arena_FlatGrounds"),
	Arena_GreatWall			UMETA(DisplayName = "Arena_GreatWall"),
	Arena_RainyDay			UMETA(DisplayName = "Arena_RainyDay"),
	Arena_StoneGarden		UMETA(DisplayName = "Arena_StoneGarden")
};

UENUM(BlueprintType)
enum class ELibraryCardSelectableState : uint8
{
	CardSelectState_None				UMETA(DisplayName = "None"),
	CardSelectState_Selectable			UMETA(DisplayName = "Selectable"),
	CardSelectState_PreviewSelec		UMETA(DisplayName = "Preview Selectable"),
	CardSelectState_Locked				UMETA(DisplayName = "Locked"),
	CardSelectState_NotSelectable		UMETA(DisplayName = "Not Selectable"),
	CardSelectState_CannotAddDeck		UMETA(DisplayName = "Cannot Add to Deck")
};

UENUM(BlueprintType)
enum class EMainMenuState : uint8
{
	MenuState_MainMenu				UMETA(DisplayName = "Main Menu"),
	MenuState_PlayOptions			UMETA(DisplayName = "Play Options"),
	MenuState_DeckSelection			UMETA(DisplayName = "Deck Selection"),
	MenuState_CreateGame			UMETA(DisplayName = "Create Game"),
	MenuState_ArenaSelection		UMETA(DisplayName = "Arena Selection"),
	MenuState_JoinFindGame			UMETA(DisplayName = "Join/Find Game"),
	MenuState_DeckBuilder			UMETA(DisplayName = "Deck Builder"),
	MenuState_Store					UMETA(DisplayName = "Store"),
	MenuState_GameOptions			UMETA(DisplayName = "GameOptions")
};
