// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "FindSessionsCallbackProxy.h"

#include "Enums.h"
#include "SystemEnums.h"

#include "WidgetInterface.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UWidgetInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API IWidgetInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReturnOption(ECardRowOptions option, const FString& cardSet, UWidget* widget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReturnSearch(const FText& searchText);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReturnCardSet(ECardSet cardSetEnum);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReturnPreview(bool previewEnabled);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReturnServerSelection(FBlueprintSessionResult sessionResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReturnDeckSelection(const FString& deckSelection);
};
