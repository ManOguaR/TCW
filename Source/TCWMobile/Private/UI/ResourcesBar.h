// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourcesBar.generated.h"

class ATCWPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResourcesBarEvent, ATCWPlayerState*, playerStateRef);

/**
 *
 */
UCLASS()
class UResourcesBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UResourcesBar(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintCallable, Category = "Events")
		FResourcesBarEvent OnUpdateManaVisual;

private:
	ATCWPlayerState* PlayerStateRef;
	int32 Mana;
	int32 Mana_Max;

private:
	UFUNCTION()
		void UpdateManaVisual(ATCWPlayerState* playerStateRef);
};
