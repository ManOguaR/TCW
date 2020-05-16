// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "LogMessage.h"
#include "Kismet/KismetTextLibrary.h"

ULogMessage::ULogMessage(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	OnLogMessage.AddDynamic(this, &ULogMessage::LogMessage);
}

void ULogMessage::LogMessage(FString message, FLinearColor specifiedColor)
{
	MessageText->SetText(FText::FromString(message));
	MessageText->SetColorAndOpacity(FSlateColor(specifiedColor));

	PlayAnimation(OnLogBegin);
}