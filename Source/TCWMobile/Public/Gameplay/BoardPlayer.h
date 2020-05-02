// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"

#include "EffectsEnums.h"

#include "BoardPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoardPlayerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerEffectSignature, EBoardPlayerEffects, effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHealthSignature, int32, health);

UCLASS()
class TCWMOBILE_API ABoardPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoardPlayer();

public:
	UPROPERTY(BlueprintCallable, Category = "Multicast Events")
		FPlayerEffectSignature OnBoardPlayerEffect;

	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FUpdateHealthSignature OnUpdateHealth;

	UPROPERTY(BlueprintCallable, Category = "Events")
		FBoardPlayerSignature OnUpdatePlayerStats;

	//TODO: REPLICATION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		int32 PlayerIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", ReplicatedUsing = OnRep_PlayerHealth)
		int32 PlayerHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		AController* OwningController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UTextRenderComponent* Health_Text;

private:
	UFUNCTION(NetMulticast, Unreliable)
		void BoardPlayerEffectEvent(EBoardPlayerEffects effect);
	UFUNCTION(Server, Reliable)
		void UpdateHealthEvent(int32 health);
	UFUNCTION(BlueprintCallable, Category = "Game Setup")
		void UpdatePlayerStatsEvent();

	UFUNCTION()
		void OnRep_PlayerHealth();

};
