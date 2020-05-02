// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "HexBoardActor.h"

// Sets default values
AHexBoardActor::AHexBoardActor(const FObjectInitializer& ObjectInitializer) : AActor(ObjectInitializer)
{
	//PRE - 11
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create and set dummy scene root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
}

// Called when the game starts or when spawned
void AHexBoardActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<AHexBoardTile*> tiles = TArray<AHexBoardTile*>();

	for (int ix = 0; ix < BoardData.BoardTiles.Num(); ix++)
	{
		AHexBoardTile* tile = NewObject<AHexBoardTile>(this);// , FString::Printf(TEXT("HexBoardTile_%d"), ix));
		//FName tileName = FString::Printf(TEXT("HexBoardTile_%d"), ix);
		//AHexBoardTile* tile = CreateDefaultSubobject<AHexBoardTile>(tileName);
		FActorSpawnParameters spawnParams;

		tile->RegisterAllComponents();

		tile->bIsHighGround = BoardData.BoardTiles[ix].IsHighGround;
		tile->bIsObstacle = BoardData.BoardTiles[ix].IsObstacle;

		tile->SetActorLocation(FVector(BoardData.BoardTiles[ix].Position.X, BoardData.BoardTiles[ix].Position.Y, this->GetActorTransform().GetLocation().Z));

		spawnParams.Template = tile;

		AHexBoardTile* spawnTile = GetWorld()->SpawnActor<AHexBoardTile>(spawnParams);
		spawnTile->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

		tiles.Add(spawnTile);

		tile->Destroy(true);
	}

	BoardTiles = tiles;
}

// Called every frame
void AHexBoardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexBoardActor::OnConstruction(const FTransform& Transform)
{
	if (BoardTiles.Num() > 0)
	{
		for (int a = 0; a < BoardTiles.Num(); a++)
		{
			BoardTiles[a]->Destroy(true);
		}
	}

	AHexBoardTile* tiletest = NewObject<AHexBoardTile>();

	if (BoardLayout == EBoard::Board_Plateau)
		BoardData = FHexBoardData(PlateauBoard, tiletest->Width / 2.0f);
	else if (BoardLayout == EBoard::Board_Desert)
		BoardData = FHexBoardData(DesertBoard, tiletest->Width / 2.0f);
	else if (BoardLayout == EBoard::Board_Swamp)
		BoardData = FHexBoardData(SwampBoard, tiletest->Width / 2.0f);
}