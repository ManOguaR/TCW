// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownTimer.generated.h"

/**
 * 
 */
UCLASS()
class TCWMOBILE_API UCountdownTimer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	int32 Timer;
};
