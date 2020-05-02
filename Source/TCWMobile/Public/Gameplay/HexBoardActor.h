// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Structs.h"
#include "HexBoardTile.h"

#include "HexBoardActor.generated.h"

UCLASS(Category = "TCW Board")
class TCWMOBILE_API AHexBoardActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHexBoardActor(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board")
		bool bAutoBuild = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board")
		float TileSpace = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board")
		FHexBoardData BoardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCW Board")
		EBoard BoardLayout;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY()
		TArray<AHexBoardTile*> BoardTiles;
};
