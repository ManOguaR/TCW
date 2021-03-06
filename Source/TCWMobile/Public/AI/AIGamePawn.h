// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIGamePawn.generated.h"

UCLASS()
class TCWMOBILE_API AAIGamePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIGamePawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
		int32 GamePlayerId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
		int32 GameAiId;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
