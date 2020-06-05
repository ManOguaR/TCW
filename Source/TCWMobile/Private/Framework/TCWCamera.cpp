// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWCamera.h"

ATCWCamera::ATCWCamera(const FObjectInitializer& ObjectInitializer) : ACameraActor(ObjectInitializer)
{
	//Pre-19
	NetUpdateFrequency = 1.0f;
}

void ATCWCamera::BeginPlay()
{
	Super::BeginPlay();
}
