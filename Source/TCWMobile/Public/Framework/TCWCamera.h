// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "TCWCamera.generated.h"

/**
 * 
 */
UCLASS()
class TCWMOBILE_API ATCWCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	ATCWCamera(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
};
