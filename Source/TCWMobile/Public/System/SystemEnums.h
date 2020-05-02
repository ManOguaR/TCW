// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAccountState : uint8
{
	Account_Unknown			UMETA(DisplayName = "Unknown"),
	Account_NotLogged		UMETA(DisplayName = "Not Logged"),
	Account_Anonymous		UMETA(DisplayName = "Anonymous Login"),
	Account_Device			UMETA(DisplayName = "Device Login"),
	Account_DeviceLinked	UMETA(DisplayName = "Device Linked Login")
};

UENUM(BlueprintType)
enum class EAccountType : uint8
{
	AccountType_Anonymous		UMETA(DisplayName = "Anonymous"),
	AccountType_Apple			UMETA(DisplayName = "App Store"),
	AccountType_Email			UMETA(DisplayName = "Email"),
	AccountType_Facebook		UMETA(DisplayName = "Facebook"),
	AccountType_Google			UMETA(DisplayName = "Google Play"),
	AccountType_Steam			UMETA(DisplayName = "Steam")
};

UENUM(BlueprintType)
enum class EEndGameResults : uint8
{
	GameResult_Victory		UMETA(DisplayName = "Victory"),
	GameResult_Defeat		UMETA(DisplayName = "Defeat"),
	GameResult_Draw			UMETA(DisplayName = "Draw")
};

UENUM(BlueprintType)
enum class EMessageType : uint8
{
	MessageType_Info		UMETA(DisplayName = "Info"),
	MessageType_Warning		UMETA(DisplayName = "Warning"),
	MessageType_Error		UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EPlatform : uint8
{
	Platform_Windows	UMETA(DisplayName = "Windows"),
	Platform_iOS		UMETA(DisplayName = "iOS"),
	Platform_Android	UMETA(DisplayName = "Android"),
	Platform_HTML5		UMETA(DisplayName = "HTML5"),
	Platform_Mac		UMETA(DisplayName = "Mac"),
	Platform_Linux		UMETA(DisplayName = "Linux")
};

//UENUM(BlueprintType)
//enum class EAlphabeticalSorting : uint8
//{
//	Sorting_AtoZ		UMETA(DisplayName = "A to Z"),
//	Sorting_ZtoA		UMETA(DisplayName = "Z to A")
//};

//UENUM(BlueprintType)
//enum class ECardRowOptions : uint8
//{
//	RowOption_None			UMETA(DisplayName = "None"),
//	RowOption_Create		UMETA(DisplayName = "Create +"),
//	RowOption_Save			UMETA(DisplayName = "Save"),
//	RowOption_Edit			UMETA(DisplayName = "Edit"),
//	RowOption_Load			UMETA(DisplayName = "Load"),
//	RowOption_Clear			UMETA(DisplayName = "Clear"),
//	RowOption_Cancel		UMETA(DisplayName = "Cancel"),
//	RowOption_Delete		UMETA(DisplayName = "Delete"),
//	RowOption_Select		UMETA(DisplayName = "Select")
//};

//UENUM(BlueprintType)
//enum class EDirection : uint8
//{
//	Direction_Left			UMETA(DisplayName = "Left"),
//	Direction_MidLeft		UMETA(DisplayName = "Center Left"),
//	Direction_MidRight		UMETA(DisplayName = "Center Right"),
//	Direction_Right			UMETA(DisplayName = "Right")
//};

//UENUM(BlueprintType)
//enum class EErrors : uint8
//{
//	Error_ValidDrop					UMETA(DisplayName = "Valid Drop"),
//	Error_NotValidPlacement			UMETA(DisplayName = "Not a Valid Placement"),
//	Error_CastToPlayerFailed		UMETA(DisplayName = "Cast Failed to Player"),
//	Error_PlacementFullNotValid		UMETA(DisplayName = "Placement Full OR Not valid Player Placement"),
//	Error_PlacementFull				UMETA(DisplayName = "Placement Full"),
//	Error_NotEnoughMana				UMETA(DisplayName = "Not Enough Mana"),
//	Error_MaxCardsPlacement			UMETA(DisplayName = "Max Cards allowed for placement"),
//	Error_PlacementCardType			UMETA(DisplayName = "Placement Does Not Support Card Type"),
//	Error_IncorrectPlacement		UMETA(DisplayName = "Incorrect Player Placement for Card"),
//	Error_InvalidPlacement			UMETA(DisplayName = "Invalid Placement"),
//	Error_NoDropLocationFound		UMETA(DisplayName = "No Supported Drop Location Found"),
//	Error_CardNotFound				UMETA(DisplayName = "Card Not Found In Hand (Server)"),
//	Error_OneInvalid				UMETA(DisplayName = "One is invalid")
//};

//UENUM(BlueprintType)
//enum class EManaCostSorting : uint8
//{
//	Sorting_LowToHigh		UMETA(DisplayName = "Lowest to Highest"),
//	Sorting_HighToLow		UMETA(DisplayName = "Highest to Lowest")
//};

//UENUM(BlueprintType)
//enum class EPriority : uint8
//{
//	Priority_None		UMETA(DisplayName = "None"),
//	Priority_Low		UMETA(DisplayName = "Low"),
//	Priority_Medium		UMETA(DisplayName = "Medium"),
//	Priority_High		UMETA(DisplayName = "High")
//};

//UENUM(BlueprintType)
//enum class ESortingTypeFilter : uint8
//{
//	Filter_Name			UMETA(DisplayName = "Name"),
//	Filter_ManaCost		UMETA(DisplayName = "ManaCost")
//};
