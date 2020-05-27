// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DisplayMessage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDisplayMessageEvent, FString, Message, FLinearColor, SpecifiedColor, float, Duration);

/**
 *
 */
UCLASS(Abstract)
class TCWMOBILE_API UDisplayMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UDisplayMessage(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION()
		void DisplayMessage(FString Message, FLinearColor SpecifiedColor, float Duration);

	//UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
	//	FDisplayMessageEvent OnDisplayMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* MessageText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* DisplaySelf;

private:
	void DisplayMessage_Delayed();
	void DisplayMessage_Callback();

};
