// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TCWHUD.generated.h"

/**
 * 
 */
UCLASS()
class TCWMOBILE_API ATCWHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATCWHUD(const FObjectInitializer& ObjectInitializer);

	void PostInitializeComponents() override;

	void BeginPlay() override;
};
