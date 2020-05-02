// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "AIGamePawn.h"

// Sets default values
AAIGamePawn::AAIGamePawn()
{
	//PRE - 03

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

