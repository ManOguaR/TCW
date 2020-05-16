// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"

#include "LogMessage.h"

#include "MessageManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMessageManagerEvent, APlayerController*, playerController, FString, message, FLinearColor, specifiedColor);

/**
 * 
 */
UCLASS()
class UMessageManager : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMessageManager(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintCallable, Category = "System Events")
		FMessageManagerEvent OnLogMessage;

protected:
	TArray<ULogMessage*> LogMessagesArray;
	int32 MaxMessagesDisplayedInLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* MessageVerticalBox;

private:
	UFUNCTION()
		void LogMessage(APlayerController* playerController, FString message, FLinearColor specifiedColor);

	void LogMessage_Continue();
};
