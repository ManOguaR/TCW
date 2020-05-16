// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "LogMessage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLogMessageEvent, FString, message, FLinearColor, specifiedColor);

/**
 *
 */
UCLASS()
class ULogMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	ULogMessage(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintCallable, Category = "System Events")
		FLogMessageEvent OnLogMessage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* OnLogBegin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* MessageText;

private:
	UFUNCTION()
		void LogMessage(FString message, FLinearColor specifiedColor);

};
