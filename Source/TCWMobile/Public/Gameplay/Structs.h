// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "Engine.h"

#include "Structs.generated.h"

UENUM(BlueprintType, Category = "TCW Board")
enum class ETileDeploymentType : uint8
{
	TileDeployment_Neutral		UMETA(DisplayName = "Neutral"),
	TileDeployment_Player1		UMETA(DisplayName = "Player 1"),
	TileDeployment_Player2		UMETA(DisplayName = "Player 2")
};

USTRUCT(BlueprintType, Atomic, Category = "TCW Board")
struct FHexTileData
{
	GENERATED_BODY()

public:
	FHexTileData(FVector2D position = FVector2D(0.0f, 0.0f),
		ETileDeploymentType deploymentType = ETileDeploymentType::TileDeployment_Neutral,
		bool isHighGround = false, bool isObstacle = false, bool isHeroOrigin = false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board Tile")
		FVector2D Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board Tile")
		ETileDeploymentType DeploymentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board Tile")
		bool IsHighGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board Tile")
		bool IsObstacle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board Tile")
		bool IsHeroOrigin;
};

USTRUCT(BlueprintType, Category = "TCW Board")
struct FHexBoardData
{
	GENERATED_BODY()

public:
	FHexBoardData();
	FHexBoardData(TArray<FHexTileData> tiles);
	FHexBoardData(TArray<TArray<uint8>> tiles, float size = 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board Tiles")
		TArray<FHexTileData> BoardTiles;
};

UENUM(BlueprintType, Category = "TCW Board")
enum class EBoardTileType :uint8
{
	TileType_Empty = 0		UMETA(DisplayName = "Empty", ToolTip = "Empty board tile."),
	TileType_Obstacle = 1	UMETA(DisplayName = "Obstacle", ToolTip = "Obstacle present in tile tile."),
	TileType_LowGround = 2	UMETA(DisplayName = "Low Ground", ToolTip = "Low ground tile."),
	TileType_HighGround = 3	UMETA(DisplayName = "High Ground", ToolTip = "High ground tile."),
	TileType_DeployArea = 4	UMETA(DisplayName = "DeployArea", ToolTip = "Deploy area tile."),
	TileType_Hole = 5		UMETA(DisplayName = "Hole", ToolTip = "Hole tile."),
	TileType_Player1 = 6	UMETA(DisplayName = "Player 1 HQ", ToolTip = "Player 1 HQ tile."),
	TileType_Player2 = 7	UMETA(DisplayName = "Player 2 HQ", ToolTip = "Player 2 HQ tile.")
};

UENUM(BlueprintType, Category = "TCW Board")
enum class EBoard :uint8
{
	Board_Plateau		UMETA(DisplayName = "Plateau"),
	Board_Desert		UMETA(DisplayName = "Desert"),
	Board_Swamp			UMETA(DisplayName = "Swamp"),
};

static TArray<TArray<uint8>> PlateauBoard = {
	{0,0,0,0,4,6,0,0},
	{0,0,0,1,4,4,4,0},
	{1,3,3,1,4,4,0,0},
	{1,3,3,3,4,4,4,0},
	{3,3,3,3,4,4,4,0},
	{0,3,3,1,1,1,4,2},
	{0,4,4,1,5,5,2,2},
	{0,0,4,4,4,4,4,2},
	{0,0,4,4,1,1,4,1},
	{0,0,0,4,4,4,4,0},
	{0,0,4,4,4,4,0,0},
	{0,0,0,7,4,0,0,0} };

static TArray<TArray<uint8>> DesertBoard = {
	{0,0,0,4,6,0,0,0},
	{0,0,4,4,4,4,0,0},
	{0,4,4,4,4,0,0,0},
	{1,4,4,4,4,4,0,0},
	{3,3,1,1,4,4,0,0},
	{3,3,5,5,3,3,0,0},
	{4,4,5,5,3,3,0,0},
	{0,4,4,1,3,3,1,1},
	{0,4,4,1,4,4,4,4},
	{0,0,4,4,4,4,4,4},
	{0,4,4,4,4,4,0,0},
	{0,0,7,4,0,0,0,0} };

static TArray<TArray<uint8>> SwampBoard = {
	{0,0,0,0,4,6,0,0},
	{0,0,0,4,4,4,0,0},
	{0,4,4,4,4,4,0,0},
	{4,4,4,4,4,4,0,0},
	{3,3,1,1,1,3,4,0},
	{3,3,3,2,2,3,4,0},
	{0,3,3,2,2,3,3,1},
	{0,1,4,4,1,4,4,1},
	{0,0,4,4,1,1,4,4},
	{0,0,4,4,4,4,4,0},
	{0,0,4,4,4,4,4,0},
	{0,0,7,4,0,0,0,0} };

