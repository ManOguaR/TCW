// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "BoardUnit.h"

// Sets default values
ABoardUnit::ABoardUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoardUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoardUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

