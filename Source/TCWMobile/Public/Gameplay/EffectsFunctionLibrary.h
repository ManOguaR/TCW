// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CardStructs.h"
#include "EffectsEnums.h"

#include "EffectsFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TCWMOBILE_API UEffectsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void GetCardEffectsForState(ECardEffects effect, FCardData cardStruct, int32 wilcardIndex, UParticleSystem* particles = nullptr, USoundCue* soundCue = nullptr);
	static void GetPlayerEffectsForState(EBoardPlayerEffects effect, UParticleSystem* particles, USoundCue* soundCue);
	static void SpawnParticleAndSoundEffects(UParticleSystem* particleToSpawn, FVector vector, AActor* orActorForLocation, FVector scale, bool autoDestroy, USoundCue* soundCueToSpawn, float volume);
};
