// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EArenaList : uint8
{
	Arena_Empty				UMETA(DisplayName = "Empty"),
	Arena_Debug				UMETA(DisplayName = "Debug Arena"),
	Arena_UrbanTheater		UMETA(DisplayName = "Urban Theater"),
	Arena_MountainRange		UMETA(DisplayName = "Mountain Range"),
	Arena_Woodlands			UMETA(DisplayName = "Woodlands"),
	Arena_DesertFront		UMETA(DisplayName = "Desert Front")
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GameState_Startup			UMETA(DisplayName = "Startup"),
	GameState_MainMenu			UMETA(DisplayName = "Main Menu"),
	GameState_LoadingScreen		UMETA(DisplayName = "Loading Screen"),
	GameState_Playing			UMETA(DisplayName = "Playing"),
	GameState_Store				UMETA(DisplayName = "Store (Coming Soon)"),
	GameState_DeckBuilding		UMETA(DisplayName = "DeckBuilding (Coming Soon)")
};

UENUM(BlueprintType)
enum class EGameTurn : uint8
{
	GameTurn_Active		UMETA(DisplayName = "Turn Active"),
	GameTurn_Inactive	UMETA(DisplayName = "Turn Inactive"),
	GameTurn_Waiting	UMETA(DisplayName = "Waiting...")
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PlayerState_Pending			UMETA(DisplayName = "Pending Action"),
	PlayerState_Interaction		UMETA(DisplayName = "Card Interaction"),
	PlayerState_Placing			UMETA(DisplayName = "Placing Card")
};

/* Card Enums */
UENUM(BlueprintType)
enum class ECardResource : uint8
{
	CardClass_Any = 0x00		UMETA(DisplayName = "Any"),
	CardClass_Bank = 1			UMETA(DisplayName = "Bank"),
	CardClass_Ore = 1 << 1		UMETA(DisplayName = "Ore"),
	CardClass_Pharma = 1 << 2	UMETA(DisplayName = "White"),
	CardClass_Power	= 1 << 3	UMETA(DisplayName = "Power"),
	CardClass_RandD = 1 << 4	UMETA(DisplayName = "R+D"),
};

UENUM(BlueprintType)
enum class ECardRarity : uint8
{
	CardRarity_Basic		UMETA(DisplayName = "Basic"),
	CardRarity_Common		UMETA(DisplayName = "Common"),
	CardRarity_UnCommon		UMETA(DisplayName = "Un-Common"),
	CardRarity_Rare			UMETA(DisplayName = "Rare"),
	//CardRarity_UltraRare	UMETA(DisplayName = "Ultra-Rare"),
	CardRarity_Legendary	UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class ECardSet : uint8
{
	CardSet_Empty		UMETA(DisplayName = "Empty"),
	CardSet_Basic		UMETA(DisplayName = "Basic Set"),
	CardSet_Debug		UMETA(DisplayName = "Debug Card Set"),
	CardSet_EmtySet		UMETA(DisplayName = "Empty Set"),
};

UENUM(BlueprintType)
enum class ECardType : uint8
{
	CardType_None		UMETA(DisplayName = "None"),
	CardType_HQ			UMETA(DisplayName = "Headquarters"),
	CardType_Site		UMETA(DisplayName = "Site"),
	CardType_Supply		UMETA(DisplayName = "Supply"),
	CardType_Order		UMETA(DisplayName = "Order"),
	CardType_Unit		UMETA(DisplayName = "Unit")
};

UENUM(BlueprintType)
enum class ECardWidgetMovementState : uint8
{
	CardMovementState_Pending		UMETA(DisplayName = "Pending"),
	CardMovementState_Hand			UMETA(DisplayName = "Move To Hand Destination"),
	CardMovementState_Preview		UMETA(DisplayName = "Move To Preview Destination"),
	CardMovementState_Return		UMETA(DisplayName = "Return From Preview Destination")
};

//UENUM(BlueprintType)
//enum class E3DCardMovementState : uint8
//{
//	CardMovementState_Placing			UMETA(DisplayName = "Placing"),
//	CardMovementState_MoveHome			UMETA(DisplayName = "Move to Home Destination"),
//	CardMovementState_Moving			UMETA(DisplayName = "Moving to Destination"),
//	CardMovementState_OnBoard			UMETA(DisplayName = "Placed On Board"),
//	CardMovementState_Attacking			UMETA(DisplayName = "Attacking"),
//	CardMovementState_Selected			UMETA(DisplayName = "Selected"),
//	CardMovementState_Graveyard			UMETA(DisplayName = "Graveyard")
//};
//
//UENUM(BlueprintType)
//enum class EAbilityTrigger : uint8
//{
//	AbilityTrigger_None				UMETA(DisplayName = "None"),
//	AbilityTrigger_OnDrop			UMETA(DisplayName = "On Drop"),
//	AbilityTrigger_TurnStart		UMETA(DisplayName = "Start of Turn"),
//	AbilityTrigger_TurnEnd			UMETA(DisplayName = "End Of Turn"),
//	AbilityTrigger_DamageTaken		UMETA(DisplayName = "Takes Damage"),
//	AbilityTrigger_Graveyard		UMETA(DisplayName = "Sent To Graveyard"),
//	AbilityTrigger_LifeExpectancy	UMETA(DisplayName = "Death by Life Expectancy")
//};
//
//UENUM(BlueprintType)
//enum class EAbilityType : uint8
//{
//	AbilityType_None				UMETA(DisplayName = "None"),
//	AbilityType_Draw				UMETA(DisplayName = "Draw Card(s)"),
//	AbilityType_IncreaseAttack		UMETA(DisplayName = "Increase Attack"),
//	AbilityType_Clone				UMETA(DisplayName = "Clone"),
//	AbilityType_IncreaseHealth		UMETA(DisplayName = "Increase Player Health"),
//	AbilityType_Retaliation			UMETA(DisplayName = "Retaliation Damage"),
//	AbilityType_DamageAll			UMETA(DisplayName = "Damage All Cards on Board"),
//	AbilityType_SpawnRandom			UMETA(DisplayName = "Spawn Random Card from Deck"),
//	AbilityType_TurnPointsAll		UMETA(DisplayName = "Give Turn Points to All Active Cards"),
//	AbilityType_TurnPoints			UMETA(DisplayName = "Increase Turn Points"),
//	AbilityType_Discard				UMETA(DisplayName = "Discard Cards In Hand"),
//	AbilityType_GraveyardBoard		UMETA(DisplayName = "Pickup first creature in the graveyard (To Board)"),
//	AbilityType_GraveyardHand		UMETA(DisplayName = "Pickup first creature in the graveyard (To Hand)"),
//	AbilityType_Possess				UMETA(DisplayName = "Possess Opponents Card"),
//	AbilityType_Swap				UMETA(DisplayName = "Swap Cards In Hand")
//};
//
//UENUM(BlueprintType)
//enum class ECardPlayConditions : uint8
//{
//	CardPlayConditions_None						UMETA(DisplayName = "None"),
//	CardPlayConditions_TurnStart				UMETA(DisplayName = "Play at start of turn (AI Only)"),
//	CardPlayConditions_TurnEnd					UMETA(DisplayName = "Play at end of Turn (AI Only)"),
//	CardPlayConditions_LowHealth				UMETA(DisplayName = "Play when Low Health (Self)"),
//	CardPlayConditions_HighHealth				UMETA(DisplayName = "Play when High Health (Self)"),
//	CardPlayConditions_CreaturesSelf			UMETA(DisplayName = "Play when creatures are on the board (Self)"),
//	CardPlayConditions_CreaturesSelfAI			UMETA(DisplayName = "Play when creatures are on the board (Self)(AI Only)"),
//	CardPlayConditions_CreaturesOpponent		UMETA(DisplayName = "Play when creatures are on the board (Opponent)"),
//	CardPlayConditions_CreaturesOpponentAI		UMETA(DisplayName = "Play when creatures are on the board (Opponent)(AI Only)"),
//	CardPlayConditions_Graveyard				UMETA(DisplayName = "Play when there are cards in the graveyard"),
//	CardPlayConditions_GraveyardAI				UMETA(DisplayName = "Play when there are cards in the graveyard (AI Only)"),
//	CardPlayConditions_LowPlayableAI			UMETA(DisplayName = "Play when low playable card in hand (AI Only)")
//};
//
//UENUM(BlueprintType)
//enum class ECardUseState : uint8
//{
//	CardUseState_InPlay			UMETA(DisplayName = "Remain In Play"),
//	CardUseState_SingleBefore	UMETA(DisplayName = "Single Use - Send to Graveyard Before Ability"),
//	CardUseState_SingleAfter	UMETA(DisplayName = "Single Use - Send to Graveyard After Ability")
//};
//
//UENUM(BlueprintType)
//enum class ECostType : uint8
//{
//	CostType_Mana		UMETA(DisplayName = "Mana"),
//	CostType_TurnPoint	UMETA(DisplayName = "Turn Point")
//};
//
//UENUM(BlueprintType)
//enum class EDamageType : uint8
//{
//	DamageType_Basic	UMETA(DisplayName = "Basic"),
//	DamageType_Poison	UMETA(DisplayName = "Poison"),
//	DamageType_Stun		UMETA(DisplayName = "Stun")
//};
//
//UENUM(BlueprintType)
//enum class EInteractionConditions : uint8
//{
//	InteractionConditions_None					UMETA(DisplayName = "None"),
//	InteractionConditions_OwnedPlacement		UMETA(DisplayName = "Card can only recieve interaction on owned placement"),
//	InteractionConditions_OppenentPlacement		UMETA(DisplayName = "Card can only recieve interaction on an oppenents placement"),
//	InteractionConditions_AttackUnits			UMETA(DisplayName = "Can Only Attack Creatures"),
//	InteractionConditions_CannotAttack			UMETA(DisplayName = "Cannot Attack if player owns card"),
//	InteractionConditions_Self					UMETA(DisplayName = "Can Recieve Self Interaction (AI Only)"),
//	InteractionConditions_SelfRemove			UMETA(DisplayName = "Can Recieve Self Interaction If does not Remove Card from Play (AI Only)")
//};
//
//UENUM(BlueprintType)
//enum class EManaAccount : uint8
//{
//	ManaAccount_None			UMETA(DisplayName = "None"),
//	ManaAccount_Placement		UMETA(DisplayName = "Placement"),
//	ManaAccount_Attack		UMETA(DisplayName = "Attack")
//};
//
//UENUM(BlueprintType)
//enum class EPlayers : uint8
//{
//	Players_Self		UMETA(DisplayName = "Self"),
//	Players_Opponent	UMETA(DisplayName = "Opponent"),
//	Players_Both		UMETA(DisplayName = "Self And Opponent")
//};
//
//UENUM(BlueprintType)
//enum class ETurnState*/ : uint8
//{
//	TurnState_Active		UMETA(DisplayName = "Turn Active"),
//	TurnState_Inactive		UMETA(DisplayName = "Turn Inactive"),
//	TurnState_Update		UMETA(DisplayName = "Update Current State")
//};