// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "MiscFunctionLibrary.h"
#include "Engine.h"

//void UMiscFunctionLibrary::Delay(AActor* callerActor, float time)
//{
//	//EXE-10
//	FTimerHandle unusedHandle;
//	callerActor->GetWorldTimerManager().SetTimer(unusedHandle, time, false);
//}
//void UMiscFunctionLibrary::Delay(UObject* callerObject, float time)
//{
//	FTimerHandle unusedHandle;
//	callerObject->GetWorld()->GetTimerManager().SetTimer(unusedHandle, time, false);
//}

UWorld* UMiscFunctionLibrary::GetWorldReference()
{
	UWorld* PIE = nullptr;
	UWorld* GamePreview = nullptr;
	UWorld* Game = nullptr;

	for (FWorldContext const& Context : GEngine->GetWorldContexts())
	{
		switch (Context.WorldType)
		{
		case EWorldType::PIE:
			PIE = Context.World();
		case EWorldType::GamePreview:
			GamePreview = Context.World();
		case EWorldType::Game:
			Game = Context.World();
		}
	}

	if (PIE)
		return PIE;
	else if (GamePreview)
		return GamePreview;
	else if (Game)
		return Game;

	return nullptr;
}

bool UMiscFunctionLibrary::CanExecuteCosmeticEvents(UObject* callerObject)
{
	return !UKismetSystemLibrary::IsDedicatedServer(callerObject);
}
