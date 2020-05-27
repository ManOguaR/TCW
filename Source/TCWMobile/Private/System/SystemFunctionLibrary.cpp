// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "SystemFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
//#include "Editor/EditorEngine.h"
//#include "Editor.h"

#include "../TCWMobile.h"
#include "DisplayMessage.h"
#include "DeckSelectionWindow.h"

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

bool USystemFunctionLibrary::IsPIE()
{
	return GIsEditor;		//&& GWorld->HasBegunPlay();
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

void USystemFunctionLibrary::ShowEndGameDialog(UObject* callerObject, FDialogWindowResponseEvent responseDelegate)
{
	//../Game/Blueprints/Widgets/Extras/DisplayMessageBP.uasset
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/DialogWindowBP.DialogWindowBP_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UDialogWindow>())
	{
		UDialogWindow* dialogWindow = CreateWidget<UDialogWindow>(UGameplayStatics::GetPlayerController(callerObject, 0), widgetClass);

		dialogWindow->OnDialogCompleted = responseDelegate;
		dialogWindow->AddToViewport(1000);
		dialogWindow->OnShowDialog.ExecuteIfBound(FDialogButtonsButtons::DialogButtons_YesNo, NSLOCTEXT("SystemStrings", "EndGameDialog_Message", "Are you sure?"));
		//popupMessage->OnDisplayMessage.Broadcast(Message, SpecifiedColor, Duration);
	}
}

void USystemFunctionLibrary::ShowDeckSelectionWindow(UObject* callerObject, FDeckSelectionResponseEvent closeDelegate)
{
	//../Game/Blueprints/Widgets/MainMenu/DeckSelectionWidget.uasset
	FStringClassReference WidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/DeckSelectionWidget.DeckSelectionWidget_C"));
	if (UClass* widgetClass = WidgetClassRef.TryLoadClass<UDeckSelectionWindow>())
	{
		UDeckSelectionWindow* deckWindow = CreateWidget<UDeckSelectionWindow>(UGameplayStatics::GetPlayerController(callerObject, 0), widgetClass);

		deckWindow->OnWindowClosed = closeDelegate;
		deckWindow->AddToViewport(1000);
		deckWindow->OnDisplayWindow.Broadcast();
	}
}

#pragma region Display System Message
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
			popupMessage->AddToViewport(1000);
			popupMessage->DisplayMessage(Message, SpecifiedColor, Duration);
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
#pragma endregion