// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "DisplayMessage.h"
#include "MiscFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UDisplayMessage::UDisplayMessage(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	//PRE - 09
	OnDisplayMessage.AddDynamic(this, &UDisplayMessage::DisplayMessageEvent);
}

void UDisplayMessage::DisplayMessageEvent(FString Message, FLinearColor SpecifiedColor, float Duration)
{
	MessageText->SetText(FText::FromString(Message));
	MessageText->SetColorAndOpacity(FSlateColor(SpecifiedColor));

	PlayAnimation(OnDisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);

	UKismetSystemLibrary::Delay(this, Duration, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, Duration);

	PlayAnimation(OnDisplaySelf, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f, false);

	UKismetSystemLibrary::Delay(this, 0.3f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.3f);

	RemoveFromParent();
}
