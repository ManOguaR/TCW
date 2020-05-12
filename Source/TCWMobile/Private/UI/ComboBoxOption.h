// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ComboBoxOption.generated.h"

/**
 * 
 */
UCLASS()
class UComboBoxOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TextBlock;
};
