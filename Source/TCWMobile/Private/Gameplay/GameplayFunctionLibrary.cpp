// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "GameplayFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

FRotator UGameplayFunctionLibrary::GetWorldRotationForPlayer(UObject* callerObject, FRotator overrideRotation, bool& cameraValid)
{
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(callerObject, 0);

	if (!cameraManager->IsValidLowLevel())
	{
		cameraValid = false;
		return FRotator(0.0f);
	}
	cameraValid = true;

	return FRotator(overrideRotation.Pitch > 0.0f ? overrideRotation.Pitch : 0.0f,
		overrideRotation.Yaw > 0.0f ? overrideRotation.Yaw : cameraManager->GetCameraRotation().Yaw,
		overrideRotation.Roll > 0.0f ? overrideRotation.Roll : 0.0f);
}
