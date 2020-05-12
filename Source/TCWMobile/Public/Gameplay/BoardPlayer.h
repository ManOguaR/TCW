// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"

#include "EffectsEnums.h"

#include "BoardPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoardPlayerEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerEffectEvent, EBoardPlayerEffects, effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHealthEvent, int32, health);

UCLASS()
class TCWMOBILE_API ABoardPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoardPlayer();

public:
	UPROPERTY(BlueprintCallable, Category = "Multicast Events")
		FPlayerEffectEvent OnBoardPlayerEffect;

	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FUpdateHealthEvent OnUpdateHealth;

	UPROPERTY(BlueprintCallable, Category = "Events")
		FBoardPlayerEvent OnUpdatePlayerStats;

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
		void BoardPlayerEffect(EBoardPlayerEffects effect);
	UFUNCTION(Server, Reliable)
		void UpdateHealth(int32 health);
	UFUNCTION(BlueprintCallable, Category = "Game Setup")
		void UpdatePlayerStats();

	UFUNCTION()
		void OnRep_PlayerHealth();

};
