// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "MessageManager.h"
#include "../TCWMobile.h"
#include "TCWGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UMessageManager::UMessageManager(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnLogMessage.AddDynamic(this, &UMessageManager::LogMessage);
}

void UMessageManager::LogMessage(APlayerController* playerController, FString message, FLinearColor specifiedColor)
{
	SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.8f));

	if (LogMessagesArray.Num() >= MaxMessagesDisplayedInLog)
	{
		LogMessagesArray[0]->RemoveFromParent();
		LogMessagesArray.RemoveAt(0);
	}

	//WidgetBlueprint'/Game/Blueprints/Widgets/Extras/LogMessageBP.LogMessageBP'
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/Extras/LogMessageBP.LogMessageBP_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<ULogMessage>())
	{
		ULogMessage* logMessage = CreateWidget<ULogMessage>(this);

		LogMessagesArray.Add(logMessage);

		//TODO: Add Game Time to String
		if (ATCWGameState* gameState = Cast<ATCWGameState>(UGameplayStatics::GetGameState(this)))
		{
			//"["+gameState->GameTimeMinutes+":"+gameState->GameTimeSeconds+"]" + message
			//gameState->GameTimeMinutes;
			//gameState->GameTimeSeconds;
		}

		logMessage->OnLogMessage.Broadcast(message, specifiedColor);

		UE_LOG(TCWLog, Log, TEXT("%s"), *message);

		MessageVerticalBox->AddChild(logMessage);
	}

	//UKismetSystemLibrary::RetriggerableDelay(this, 5.0f, FLatentActionInfo());
	//SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UMessageManager::LogMessage_Continue, 5.0f, true);
}

void UMessageManager::LogMessage_Continue()
{
	SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
}
