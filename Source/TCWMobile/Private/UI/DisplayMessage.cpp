// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DisplayMessage.h"
#include "MiscFunctionLibrary.h"

#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

UDisplayMessage::UDisplayMessage(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	//PRE - 09
	//OnDisplayMessage.AddDynamic(this, &UDisplayMessage::DisplayMessage);
}

void UDisplayMessage::DisplayMessage(FString Message, FLinearColor SpecifiedColor, float Duration)
{
	MessageText->SetText(FText::FromString(Message));
	MessageText->SetColorAndOpacity(FSlateColor(SpecifiedColor));

	PlayAnimation(DisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);

	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UDisplayMessage::DisplayMessage_Delayed, 3.0f);
}
void UDisplayMessage::DisplayMessage_Delayed()
{
	PlayAnimation(DisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f, false);

	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UDisplayMessage::DisplayMessage_Callback, 0.3f);
}
void UDisplayMessage::DisplayMessage_Callback()
{
	RemoveFromParent();
}