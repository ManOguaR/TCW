// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CardStructs.h"

#include "Graveyard.generated.h"

class ABoardUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGraveyardEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FToGraveyardEvent, ABoardUnit*, boardUnit, FName, name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFromGraveyardEvent, FName, card, int32, index);

UCLASS()
class TCWMOBILE_API AGraveyard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraveyard();

	UPROPERTY(BlueprintCallable)
		FToGraveyardEvent OnAddToGraveyard;
	UPROPERTY(BlueprintCallable)
		FGraveyardEvent OnAddToGraveyardVisual;
	UPROPERTY(BlueprintCallable)
		FFromGraveyardEvent OnRemoveCardFromGraveyard;

	UPROPERTY(EditAnywhere)
		int32 PlayerIndex;

private:
	TArray<ABoardUnit*> ActorsInGraveyard;
	TArray<FName> GraveyardList;
	TArray<FCardData> GraveyardStructList;
	int32 RemoveIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void AddToGraveyard(ABoardUnit* boardUnit, FName name);
	UFUNCTION()
		void AddToGraveyardVisual();
	UFUNCTION()
		void RemoveCardFromGraveyardEvent(FName card, int32 index);
	void RemoveCardFromGraveyard(FName card, int32 index);
	void HideUnseenCardsInGraveyard();
	void RecreateTopGraveyardCardActor();
	FVector GetUnitInGraveyardLocation(int32 add);
};
