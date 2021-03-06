// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "MiscFunctionLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMiscFunctionLibrary::UMiscFunctionLibrary(const FObjectInitializer& ObjectInitializer) : UBlueprintFunctionLibrary(ObjectInitializer)
{

}

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

FWidgetTransform UMiscFunctionLibrary::InterpToGoalLocation2D(FWidgetTransform currentTransform, FWidgetTransform goalTransform, float deltaTime, float interpSpeed, bool& arrived)
{
	FVector2D translation = FMath::Vector2DInterpTo(currentTransform.Translation, goalTransform.Translation, deltaTime, interpSpeed);
	FVector2D scale = FMath::Vector2DInterpTo(currentTransform.Scale, goalTransform.Scale, deltaTime, interpSpeed);
	float rotation = FMath::FInterpTo(currentTransform.Angle, goalTransform.Angle, deltaTime, interpSpeed);

	FVector2D tDir, sDir, cTtDir, cTsDir;
	float tLen, sLen, cTtLen, cTsLen;

	translation.ToDirectionAndLength(tDir, tLen);
	scale.ToDirectionAndLength(sDir, sLen);
	goalTransform.Translation.ToDirectionAndLength(cTtDir, cTtLen);
	goalTransform.Scale.ToDirectionAndLength(cTsDir, cTsLen);

	arrived = FMath::IsNearlyEqual(FMath::Abs(tLen), FMath::Abs(cTtLen), 0.001f) && FMath::IsNearlyEqual(FMath::Abs(sLen), FMath::Abs(cTsLen), 0.001f) && FMath::IsNearlyEqual(FMath::Abs(rotation), FMath::Abs(goalTransform.Angle), 0.001f);

	return FWidgetTransform(arrived ? goalTransform.Translation : translation, arrived ? goalTransform.Scale : scale, FVector2D(0.0f), arrived ? goalTransform.Angle : rotation);
}

float UMiscFunctionLibrary::ModifyDPIScaling(UObject* callerObject, float value, bool invertDPIScaling)
{
	return (invertDPIScaling ? (value * ((1.0f - UWidgetLayoutLibrary::GetViewportScale(callerObject)) + 1.0f)) : (value / ((1.0f - UWidgetLayoutLibrary::GetViewportScale(callerObject)) + 1.0f)));
}

FVector2D UMiscFunctionLibrary::GetMousePositionInRange(UObject* callerObject, FVector2D sizeOffset, float globalEdgeOffset, float XEdgeOffset, float YEdgeOffset)
{
	float locationX, locationY;
	UGameplayStatics::GetPlayerController(callerObject, 0)->GetMousePosition(locationX, locationY);
	FVector2D viewPortSize = UWidgetLayoutLibrary::GetViewportSize(callerObject);
	float literal = UKismetSystemLibrary::MakeLiteralFloat(globalEdgeOffset);


	return FVector2D(FMath::Clamp(locationX, literal, ((viewPortSize.X - literal) - (sizeOffset.X + XEdgeOffset))), (FMath::Clamp(locationY, (literal + sizeOffset.Y), (viewPortSize.Y - literal) - (sizeOffset.Y - YEdgeOffset)) - YEdgeOffset));
}
