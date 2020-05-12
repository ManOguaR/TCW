// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "SystemFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

#include "../TCWMobile.h"
#include "DisplayMessage.h"

USystemFunctionLibrary::USystemFunctionLibrary(const FObjectInitializer& ObjectInitializer) : UBlueprintFunctionLibrary(ObjectInitializer)
{
	//PRE - 18
}

UTCWGameInstance* USystemFunctionLibrary::GetTCWGameInstance(UObject* callerObject)
{
	//EXE-11
	if (UTCWGameInstance* result = Cast<UTCWGameInstance>(UGameplayStatics::GetGameInstance(callerObject)))
		return result;
	
	UE_LOG(TCWLogErrors, Fatal, TEXT("Cast to UTCWGameInstance from UGameInstance."));
	return nullptr;
}

FString USystemFunctionLibrary::GetProjectVersion()
{
	FString projectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		projectVersion,
		GGameIni);
	return projectVersion;
}

void USystemFunctionLibrary::DisplayError(UObject* callerObject, FString Message, float Duration)
{
	USystemFunctionLibrary::DisplaySystemMessage(callerObject, Message,
		FLinearColor(FColor::Red), Duration, true, true, EMessageType::MessageType_Error);
}
void USystemFunctionLibrary::DisplayMessage(UObject* callerObject, FString Message, float Duration)
{
	USystemFunctionLibrary::DisplaySystemMessage(callerObject, Message,
		FLinearColor(FColor::White), Duration);
}
void USystemFunctionLibrary::DisplayWarning(UObject* callerObject, FString Message, float Duration)
{
	USystemFunctionLibrary::DisplaySystemMessage(callerObject, Message,
		FLinearColor(FColor::Yellow), Duration, true, true, EMessageType::MessageType_Warning);
}

void USystemFunctionLibrary::DisplaySystemMessage(UObject* callerObject, FString Message, FLinearColor SpecifiedColor, float Duration, bool ToScreen, bool ToLog, EMessageType MsgType)
{
	if (ToScreen)
	{
		//../Game/Blueprints/Widgets/Extras/DisplayMessageBP.uasset
		FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/Extras/DisplayMessageBP.DisplayMessageBP_C"));
		if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UDisplayMessage>())
		{
			UDisplayMessage* popupMessage = CreateWidget<UDisplayMessage>(UGameplayStatics::GetPlayerController(callerObject, 0), widgetClass);
			popupMessage->OnDisplayMessage.Broadcast(Message, SpecifiedColor, Duration);
		}
	}
	if (ToLog)
	{
		if (MsgType == EMessageType::MessageType_Info)
		{
			UE_LOG(TCWLog, Log, TEXT("%s"), *Message);
		}
		else if (MsgType == EMessageType::MessageType_Warning)
		{
			UE_LOG(TCWLog, Warning, TEXT("%s"), *Message);
		}
		else if (MsgType == EMessageType::MessageType_Error)
		{
			UE_LOG(TCWLogErrors, Error, TEXT("%s"), *Message);
		}
	}

}