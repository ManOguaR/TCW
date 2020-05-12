// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HexBoardTile.generated.h"

UCLASS(Category = "TCW Board")
class TCWMOBILE_API AHexBoardTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexBoardTile();

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* HexagonMeshComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* ObstacleMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		UStaticMesh* LowGroundMesh;
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		UStaticMesh* HighGroundMesh;
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		UMaterial* Material;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		bool bIsHighGround;
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		bool bIsObstacle;

	UPROPERTY(EditAnywhere, Category = "Hexagon Tile")
		bool bIsFlat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hexagon Tile")
		float Width;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void OnConstruction(const FTransform& Transform) override;
};
