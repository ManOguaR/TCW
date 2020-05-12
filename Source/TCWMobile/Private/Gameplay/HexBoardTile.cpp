// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "HexBoardTile.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"

// Sets default values
AHexBoardTile::AHexBoardTile()
{
	//PRE - 12
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create and set dummy scene root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	//Create and set HexagonMeshComponent
	HexagonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HexagonMeshComponent"));
	HexagonMeshComponent->SetupAttachment(RootComponent);

	//..Game/Meshes/Board/HexagonTile.uasset 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LowGroundMeshAsset(TEXT("StaticMesh'/Game/Meshes/Board/HexagonTile.HexagonTile'"));
	LowGroundMesh = LowGroundMeshAsset.Object;

	Width = FMath::RoundToFloat(LowGroundMesh->GetBounds().GetBox().GetSize().X);

	//..Game/Meshes/Board/HexagonTile_High.uasset
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HighGroundMeshAsset(TEXT("StaticMesh'/Game/Meshes/Board/HexagonTile_High.HexagonTile_High'"));
	HighGroundMesh = HighGroundMeshAsset.Object;

	//Create and set ObstacleMeshComponent
	ObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMeshComponent"));
	//..Game/Meshes/Board/HexagonTile_High.uasset
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObstacleMeshAsset(TEXT("StaticMesh'/Game/Meshes/Board/HexagonObstacle.HexagonObstacle'"));
	ObstacleMeshComponent->SetStaticMesh(ObstacleMeshAsset.Object);
	ObstacleMeshComponent->SetupAttachment(HexagonMeshComponent);
}

// Called when the game starts or when spawned
void AHexBoardTile::BeginPlay()
{
	Super::BeginPlay();

	//if (Material->IsValidLowLevel())
	//{
	//	HexagonMeshComponent->GetStaticMesh()->SetMaterial(0, Material);
	//}

}

void AHexBoardTile::OnConstruction(const FTransform& Transform)
{
	HexagonMeshComponent->SetStaticMesh(bIsHighGround ? HighGroundMesh : LowGroundMesh);
	HexagonMeshComponent->SetRelativeRotation(FRotator(0.0f, bIsFlat ? 90.0f : 0.0f, 0.0f));

	ObstacleMeshComponent->SetVisibility(bIsObstacle);
	ObstacleMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, bIsHighGround ? 50.0f : 0.0f));

	//if (Material->IsValidLowLevel())
	//{
	//	HexagonMeshComponent->GetStaticMesh()->SetMaterial(0, Material);
	//}

}
