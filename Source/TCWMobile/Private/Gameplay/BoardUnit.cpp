// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "BoardUnit.h"
#include "../TCWMobile.h"
#include "DeckFunctionLibrary.h"
#include "GameplayFunctionLibrary.h"
#include "MiscFunctionLibrary.h"

#include "Kismet/KismetStringLibrary.h"

// Sets default values
ABoardUnit::ABoardUnit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnSetUnitVisuals.AddDynamic(this, &ABoardUnit::MulticastSetUnitVisuals);
	OnRemoveUnitActor.AddDynamic(this, &ABoardUnit::RemoveUnitActor);
	OnSetUnitHiddenState.AddDynamic(this, &ABoardUnit::SetUnitHiddenState);
	OnServerSetUnitData.AddDynamic(this, &ABoardUnit::ServerSetUnitData);

	OnForceMoveCardDirectlyToGraveyard.AddDynamic(this, &ABoardUnit::ForceMoveCardDirectlyToGraveyard);

	OnOpenGate.AddDynamic(this, &ABoardUnit::OpenGate);
	OnCloseGate.AddDynamic(this, &ABoardUnit::CloseGate);

	OnArrivedAtDestination.AddDynamic(this, &ABoardUnit::ArrivedAtDestination);
}

void ABoardUnit::TakesDamage_Implementation(ABoardUnit* dealingUnit, int32 damageAttack, int32 damageHealth)
{

}

void ABoardUnit::OnActivePlayerTurn_Implementation()
{

}

void ABoardUnit::OnEndActivePlayerTurn_Implementation()
{

}

// Called when the game starts or when spawned
void ABoardUnit::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoardUnit::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	DeltaTime = deltaTime;

	if (bIsGateOpen)
	{
		bool bLocationArrived, bRotationArrived;
		SetActorTransform(FTransform(InterpSelfToRotation(bRotationArrived), InterpSelfToLocation(bLocationArrived)));

		if (bLocationArrived && bRotationArrived)
		{
			OnArrivedAtDestination.Broadcast();
		}
	}
}

void ABoardUnit::RemoveUnitActor()
{
	FTimerHandle unusedHandle;
	GetWorldTimerManager().SetTimer(unusedHandle, this, &ABoardUnit::RemoveUnitActor_Delayed, 0.5f);
}

void ABoardUnit::RemoveUnitActor_Delayed()
{
	this->K2_DestroyActor();
}

void ABoardUnit::SetUnitHiddenState(bool newHidden)
{
	SetActorHiddenInGame(newHidden);
	UnitBase->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABoardUnit::ServerSetUnitData(FName cardName, ECardSet unitSet, FCardData dataStruct, bool setUnitDataFromStruct)
{
	if (HasAuthority())
	{
		SetUnitData(cardName, unitSet, dataStruct, setUnitDataFromStruct);
		OnSetUnitVisuals.Broadcast();
	}
}

void ABoardUnit::MulticastSetUnitVisuals()
{
	SetUnitVisuals();
}

void ABoardUnit::MulticastUpdateVisualStats()
{
	if (IsValidLowLevel())
	{
		AttackText->SetText(FText::FromString(FString::FromInt(Attack)));
		HealthText->SetText(FText::FromString(FString::FromInt(Health)));
		CostText->SetText(FText::FromString(FString::FromInt(PlacementCost)));
	}
}

void ABoardUnit::ForceMoveCardDirectlyToGraveyard(AGraveyard* graveyard)
{
	bool bIsValid;
	SetActorRotation(UGameplayFunctionLibrary::GetWorldRotationForPlayer(this, FRotator(0.0f, 0.0f, 0.0f), bIsValid));

	UnitBase->SetMaterial(0, OuterMaterial);

	//PlayMovementAnimation();
}

void ABoardUnit::OpenGate()
{
	bIsGateOpen = true;
}
void ABoardUnit::CloseGate()
{
	bIsGateOpen = false;
}

void ABoardUnit::SetUnitData(FName cardName, ECardSet cardSet, FCardData dataStruct, bool setUnitDataFromStruct)
{
	if (setUnitDataFromStruct)
	{
		CardDataStruct = dataStruct;
	}
	else
	{
		CardDataStruct = UDeckFunctionLibrary::GetCardData(cardName, cardSet);
	}

	CardDataTableName = cardName;
	CardSet = cardSet;
	Name = CardDataStruct.Name;
	//TODO: w/notify
	Attack = CardDataStruct.Attack.Damage;
	CanAttackPlayers = CardDataStruct.Attack.CanAttackPlayer;
	CanAttackCards = CardDataStruct.Attack.CanAttackCards;
	//TODO: w/notify
	Health = CardDataStruct.Health.Health;
	LifeExpectancy = CardDataStruct.Health.LifeExpectancy;
	bEnableLifeExpectancy = LifeExpectancy > 0;
	//TODO: w/notify
	PlacementCost = CardDataStruct.PlacementSettings.Cost;
	//TODO: w/notify
	Type = CardDataStruct.Type;

	for (FCardAbility_Struct each : CardDataStruct.Abilities)
	{
		AbilityStructs.Add(each);
		AbilityTriggers.Add(each.Trigger);
		AbilityTypes.Add(each.Type);
	}

	Description = CardDataStruct.Description.Description;
}

void ABoardUnit::SetUnitVisuals()
{
	if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
	{
		CardFrameMaterial = UnitBase->CreateDynamicMaterialInstance(1, CardDataStruct.Visual.FrameMaterial);
		UnitBase->SetMaterial(3, CardFrameMaterial);
		CardImageMaterial = UnitBase->CreateDynamicMaterialInstance(2, CardDataStruct.Visual.ImageMaterial);
		UnitBase->SetMaterial(1, CardImageMaterial);
		CardBackMaterial = UnitBase->CreateDynamicMaterialInstance(2, CardDataStruct.Visual.BackMaterial);
		UnitBase->SetMaterial(2, CardBackMaterial);

		NameText->SetText(Name);
		UnitTypeText->SetText(GETENUMTEXT("ECardType", Type));

		GenerateDescription(CardDataStruct.Description.Description);

		OnUpdateVisualStats.Broadcast();

		OuterMaterial = ShadowMaterialRef;
	}
}

void ABoardUnit::GenerateDescription(FString descriptionText)
{
	uint32 currentStringCharacter = 0;

	for (int32 index = 0; index < (UKismetStringLibrary::GetCharacterArrayFromString(descriptionText).Num() / MaxDescriptionCharacterLineLength); index++)
	{
		if (index == 0)
		{
			DescriptionText->SetText(FText::FromString(UKismetStringLibrary::GetSubstring(descriptionText, currentStringCharacter, MaxDescriptionCharacterLineLength)));
		}
		else
		{
			//AddTextRenderComponent
			UTextRenderComponent* currentComponent = NewObject<UTextRenderComponent>(this);

			currentComponent->SetRelativeTransform(FTransform(DescriptionText->GetRelativeTransform().GetRotation(), FVector(DescriptionText->GetRelativeTransform().GetLocation().X + (((index * 10.0f) + 15.0f) * -1.0f), DescriptionText->GetRelativeTransform().GetLocation().Y, DescriptionText->GetRelativeTransform().GetLocation().Z), DescriptionText->GetRelativeTransform().GetScale3D()));
			currentComponent->SetTextRenderColor(DescriptionText->TextRenderColor);
			currentComponent->SetWorldSize(DescriptionText->WorldSize);
			currentComponent->SetText(FText::FromString(UKismetStringLibrary::GetSubstring(descriptionText, currentStringCharacter, MaxDescriptionCharacterLineLength)));

			currentComponent->SetupAttachment(RootComponent);
		}
		currentStringCharacter = currentStringCharacter + MaxDescriptionCharacterLineLength;
	}
}

void ABoardUnit::PlayMovementAnimation(E3DCardMovementState cardMovementState, FVector manualGoalDestination, FRotator manualGoalRotator, AActor* orGoalActor, float zOffset, float errorTolerance, float interSpeed)
{
	CardMovementState = cardMovementState;

	if (orGoalActor && orGoalActor->IsValidLowLevel())
	{
		GoalDestination = FVector(orGoalActor->GetActorLocation().X, orGoalActor->GetActorLocation().Y, orGoalActor->GetActorLocation().Z + zOffset);
		GoalRotation = orGoalActor->GetActorRotation();
	}
	else
	{
		GoalDestination = manualGoalDestination;
		GoalRotation = manualGoalRotator;
	}
	ErrorTolerance = errorTolerance;
	InterpSpeed = interSpeed;

	OnOpenGate.Broadcast();
}

FRotator ABoardUnit::InterpSelfToRotation(bool& arrived)
{
	arrived = GetActorRotation().Equals(GoalRotation, ErrorTolerance);

	return arrived ? GoalRotation : FMath::RInterpTo(GetActorRotation(), GoalRotation, DeltaTime, InterpSpeed / 5.0f);
}

FVector ABoardUnit::InterpSelfToLocation(bool& arrived)
{
	arrived = FMath::IsNearlyEqual((GetActorLocation() - GoalDestination).Size(), 0.0f, ErrorTolerance);

	return arrived ? GoalDestination : FMath::VInterpTo(GetActorLocation(), GoalDestination, DeltaTime, InterpSpeed);
}

void ABoardUnit::ArrivedAtDestination()
{
	switch (CardMovementState)
	{
	case E3DCardMovementState::CardMovementState_MoveHome:
	{
		//TODO:
		break;
	}
	case E3DCardMovementState::CardMovementState_Attacking:
	{
		bIsAttacking = false;
		//TODO:
		break;
	}
	case E3DCardMovementState::CardMovementState_Graveyard:
	{
		OnCloseGate.Broadcast();
		//if (PlacementOwner->IsValidLowLevel())
		//{
		//	PlacementOwner->GraveyardRef->OnAddToGraveyardVisual.Broadcast();
		//}
		break;
	}
	case E3DCardMovementState::CardMovementState_Placing:
	case E3DCardMovementState::CardMovementState_Moving:
	case E3DCardMovementState::CardMovementState_OnBoard:
	case E3DCardMovementState::CardMovementState_Selected:
	default:
		break;
	}
}
