// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DisplayMessage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDisplayMessageSignature, FString, Message, FLinearColor, SpecifiedColor, float, Duration);

/**
 *
 */
UCLASS(Abstract)
class TCWMOBILE_API UDisplayMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UDisplayMessage(const FObjectInitializer& ObjectInitializer);

private:
	UFUNCTION()
	void DisplayMessageEvent(FString Message, FLinearColor SpecifiedColor, float Duration);

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
	FDisplayMessageSignature OnDisplayMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MessageText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* OnDisplaySelf;
};
