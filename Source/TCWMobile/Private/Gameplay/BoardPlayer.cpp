// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "BoardPlayer.h"
#include "Net\UnrealNetwork.h"

#include "MiscFunctionLibrary.h"
#include "Kismet/KismetTextLibrary.h"

// Sets default values
ABoardPlayer::ABoardPlayer()
{
	//PRE - 05

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnBoardPlayerEffect.AddDynamic(this, &ABoardPlayer::BoardPlayerEffect);
	OnUpdateHealth.AddDynamic(this, &ABoardPlayer::UpdateHealth);
	OnUpdatePlayerStats.AddDynamic(this, &ABoardPlayer::UpdatePlayerStats);
}

// Called when the game starts or when spawned
void ABoardPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void ABoardPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ABoardPlayer, PlayerHealth);
}

// Called every frame
void ABoardPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardPlayer::BoardPlayerEffect_Implementation(EBoardPlayerEffects effect)
{

}

void ABoardPlayer::UpdatePlayerStats()
{
	if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
	{
		Health_Text->SetText(UKismetTextLibrary::Conv_IntToText(PlayerHealth));
	}
}

void ABoardPlayer::UpdateHealth_Implementation(int32 health)
{
	if (health > PlayerHealth) //INCREASE
	{
	}
	else if (health < PlayerHealth) //DECREASE
	{
	}
	else //UNCHANGED
	{

	}
}

void ABoardPlayer::OnRep_PlayerHealth()
{

}

