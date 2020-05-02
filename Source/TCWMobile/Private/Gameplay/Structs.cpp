// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "Structs.h"

FHexTileData::FHexTileData(FVector2D position, ETileDeploymentType deploymentType, bool isHighGround, bool isObstacle, bool isHeroOrigin) :
	Position(position),
	DeploymentType(deploymentType),
	IsHighGround(isHighGround),
	IsObstacle(isObstacle),
	IsHeroOrigin(isHeroOrigin)
{

}

FHexBoardData::FHexBoardData()
{
	//PRE - 28
}
FHexBoardData::FHexBoardData(TArray<FHexTileData> tiles) :
	BoardTiles(tiles)
{

}

FHexBoardData::FHexBoardData(TArray<TArray<uint8>> tiles, float radius)
{
	TArray<FHexTileData> tileMap = TArray<FHexTileData>();

	for (int i = 0; i < tiles.Num(); i++)
	{
		for (int j = 0; j < tiles[i].Num(); j++)
		{
			float posX = i * ((radius / 2.0f) * 3.0f);
			float offsetY = (FMath::Sqrt(3.0f) * radius) / 2.0f;
			float posY = (j * (FMath::Sqrt(3.0f) * radius)) + ((i % 2 == 0) ? offsetY : 0.0f);
			switch ((EBoardTileType)tiles[i][j])
			{
			case EBoardTileType::TileType_LowGround:
				tileMap.Add(FHexTileData(FVector2D(posX, posY), ETileDeploymentType::TileDeployment_Neutral, false, false, false));
				break;
			case EBoardTileType::TileType_HighGround:
				tileMap.Add(FHexTileData(FVector2D(posX, posY), ETileDeploymentType::TileDeployment_Neutral, true, false, false));
				break;
			case EBoardTileType::TileType_DeployArea:
				tileMap.Add(FHexTileData(FVector2D(posX, posY), i < tiles.Num() / 2 ? ETileDeploymentType::TileDeployment_Player1 : ETileDeploymentType::TileDeployment_Player2, false, false, false));
				break;
			case EBoardTileType::TileType_Player1:
				tileMap.Add(FHexTileData(FVector2D(posX, posY), ETileDeploymentType::TileDeployment_Player1, false, false, true));
				break;
			case EBoardTileType::TileType_Player2:
				tileMap.Add(FHexTileData(FVector2D(posX, posY), ETileDeploymentType::TileDeployment_Player2, false, false, true));
				break;
			case EBoardTileType::TileType_Obstacle:
				tileMap.Add(FHexTileData(FVector2D(posX, posY), ETileDeploymentType::TileDeployment_Neutral, false, true, false));
				break;
			case EBoardTileType::TileType_Hole:
			case EBoardTileType::TileType_Empty:
			default:
				break;
			}
		}
	}

	BoardTiles = tileMap;
}
