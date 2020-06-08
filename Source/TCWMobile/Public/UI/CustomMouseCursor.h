// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "CustomMouseCursor.generated.h"

/**
 * 
 */
UCLASS()
class TCWMOBILE_API UCustomMouseCursor : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CursorImage;
};
