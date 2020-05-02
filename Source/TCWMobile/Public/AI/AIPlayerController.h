// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BoardPlayer.h"

#include "AIPlayerController.generated.h"

UCLASS()
class TCWMOBILE_API AAIPlayerController : public AAIController
{
	GENERATED_BODY()

public:
	AAIPlayerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Player")
		ABoardPlayer* BoardPlayerRef;

};