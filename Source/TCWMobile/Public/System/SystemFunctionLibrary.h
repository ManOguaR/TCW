// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SystemEnums.h"
#include "TCWGameInstance.h"

#include "SystemFunctionLibrary.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TCWMOBILE_API USystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	USystemFunctionLibrary(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		static UTCWGameInstance* GetTCWGameInstance(UObject* callerObject);

	//UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Debug?"), Category = "System Info")
	//	static bool IsDebug();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Play In Editor?"), Category = "System Info")
		static bool IsPIE();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Project Version"), Category = "System Info")
		static FString GetProjectVersion();

	UFUNCTION(BlueprintCallable)
		static void DisplayError(UObject* callerObject, FString Message, float Duration = 3.0f);
	UFUNCTION(BlueprintCallable)
		static void DisplayMessage(UObject* callerObject, FString Message, float Duration = 3.0f);
	UFUNCTION(BlueprintCallable)
		static void DisplayWarning(UObject* callerObject, FString Message, float Duration = 3.0f);

	UFUNCTION(BlueprintCallable)
		static void DisplaySystemMessage(UObject* callerObject, FString Message, FLinearColor SpecifiedColor, float Duration, bool ToScreen = true, bool ToLog = false, EMessageType MsgType = EMessageType::MessageType_Info);

};
