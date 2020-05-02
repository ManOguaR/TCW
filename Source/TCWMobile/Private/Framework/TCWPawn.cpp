// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWPawn.h"

// Sets default values
ATCWPawn::ATCWPawn(const FObjectInitializer& ObjectInitializer) : APawn(ObjectInitializer)
{	
	//Pre-24
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATCWPawn::BeginPlay()
{
	//NO EXE UNTIL SPAWN
	Super::BeginPlay();
	
}

// Called every frame
void ATCWPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATCWPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

