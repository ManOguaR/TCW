// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "EffectsFunctionLibrary.h"

void UEffectsFunctionLibrary::GetCardEffectsForState(ECardEffects effect, FCardData cardStruct, int32 wilcardIndex, UParticleSystem* particles, USoundCue* soundCue)
{
	switch (effect)
	{
	case ECardEffects::CardEffects_OnCardPlaced:
	{
		particles = cardStruct.PlacementSettings.DeployParticle;
		soundCue = cardStruct.PlacementSettings.DeploySoundCue;
		break;
	}
	case ECardEffects::CardEffects_OnAttack:
	{
		particles = cardStruct.Attack.AttackParticle;
		soundCue = cardStruct.Attack.AttackSoundCue;
		break;
	}
	case ECardEffects::CardEffects_OnAbilityUsed:
	{
		particles = cardStruct.Abilities[wilcardIndex].Particle;
		soundCue = cardStruct.Abilities[wilcardIndex].SoundCue;
		break;
	}
	case ECardEffects::CardEffects_OnDeath:
	{
		particles = cardStruct.Health.DeathParticle;
		soundCue = cardStruct.Health.DeathSoundCue;
		break;
	}
	case ECardEffects::CardEffects_DecreasedHealth:
	case ECardEffects::CardEffects_IncreasedHealth:
	default:
		break;
	}
}

void UEffectsFunctionLibrary::GetPlayerEffectsForState(EBoardPlayerEffects effect, UParticleSystem* particles, USoundCue* soundCue)
{
	switch (effect)
	{
	case EBoardPlayerEffects::PlayerEffects_DecreasedHealth:
	{
		particles = nullptr;
		soundCue = nullptr;
		break;
	}
	case EBoardPlayerEffects::PlayerEffects_IncreasedHealth:
	{
		particles = nullptr;
		soundCue = nullptr;
		break;
	}
	case EBoardPlayerEffects::PlayerEffects_Death:
	{
		particles = nullptr;
		soundCue = nullptr;
		break;
	}
	default:
		break;
	}
}

void UEffectsFunctionLibrary::SpawnParticleAndSoundEffects(UParticleSystem* particleToSpawn, FVector vector, AActor* orActorForLocation, FVector scale, bool autoDestroy, USoundCue* soundCueToSpawn, float volume)
{
	if (particleToSpawn->IsValidLowLevel())
	{
		UGameplayStatics::SpawnEmitterAtLocation(nullptr, particleToSpawn, FTransform(orActorForLocation->IsValidLowLevel()? orActorForLocation->GetActorLocation(): vector), autoDestroy)->SetWorldScale3D(scale);
	}
	if (soundCueToSpawn->IsValidLowLevel())
	{
		UGameplayStatics::SpawnSound2D(nullptr, soundCueToSpawn, volume);
	}
}
