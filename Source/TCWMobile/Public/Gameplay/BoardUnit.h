// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CardInteractionInterface.h"
#include "CardStructs.h"

#include "BoardUnit.generated.h"

class AGraveyard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoardUnitEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHideBoardUnitEvent, bool, newHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGraveyardUnitEvent, AGraveyard*, graveyard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSetUnitDataEvent, FName, cardName, ECardSet, unitSet, FCardData, dataStruct, bool, setUnitDataFromStruct);

UCLASS()
class TCWMOBILE_API ABoardUnit : public AActor, public ICardInteractionInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FBoardUnitEvent OnSetUnitVisuals;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FBoardUnitEvent OnRemoveUnitActor;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FHideBoardUnitEvent OnSetUnitHiddenState;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FSetUnitDataEvent OnServerSetUnitData;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FBoardUnitEvent OnUpdateVisualStats;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FGraveyardUnitEvent OnForceMoveCardDirectlyToGraveyard;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FBoardUnitEvent OnOpenGate;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FBoardUnitEvent OnCloseGate;
	UPROPERTY(EditAnywhere, Category = "Unit Events")
		FBoardUnitEvent OnArrivedAtDestination;

	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterial* OuterMaterial;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterial* ShadowMaterialRef;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterialInstance* CardFrameMaterial;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterialInstance* CardFrameMaterial_Instance;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterialInstance* CardImageMaterial;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterialInstance* CardImageMaterial_Instance;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UUserWidget* ViewCardPreview;
	UPROPERTY(EditAnywhere, Category = "Unit Visuals")
		UMaterialInstance* CardBackMaterial;

	UPROPERTY(EditAnywhere, Category = "Unit Data")
		FCardData CardDataStruct;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		FName CardDataTableName;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		FText Name;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		int32 Attack;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		int32 Health;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		int32 PlacementCost;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		int32 TurnPoints;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		int32 LifeExpectancy;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		int32 TurnsAlive;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		ECardType Type;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		ECardSet CardSet;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		FString Description;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		bool CanAttackPlayers;
	UPROPERTY(EditAnywhere, Category = "Unit Data")
		bool CanAttackCards;

	UPROPERTY(EditAnywhere, Category = "System")
		float DeltaTime;
	UPROPERTY(EditAnywhere, Category = "System")
		float ErrorTolerance;
	UPROPERTY(EditAnywhere, Category = "System")
		float InterpSpeed;
	UPROPERTY(EditAnywhere, Category = "System")
		int32 MaxDescriptionCharacterLineLength = 43;
	UPROPERTY(EditAnywhere, Category = "System")
		E3DCardMovementState CardMovementState;
	UPROPERTY(EditAnywhere, Category = "System")
		bool bIsAttacking;
	UPROPERTY(EditAnywhere, Category = "System")
		bool bEnableLifeExpectancy;
	UPROPERTY(BlueprintSetter = SetOwningPlayerID, BlueprintGetter = GetOwningPlayerID, Category = "System")
		int32 OwningPlayerID;

	UPROPERTY(EditAnywhere, Category = "Abilities")
		TArray<EAbilityTrigger> AbilityTriggers;
	UPROPERTY(EditAnywhere, Category = "Abilities")
		TArray<EAbilityType> AbilityTypes;
	UPROPERTY(EditAnywhere, Category = "Abilities")
		TArray<FCardAbility_Struct> AbilityStructs;

	UPROPERTY(EditAnywhere, Category = "Misc Variables")
		FVector HomeDestination;
	UPROPERTY(EditAnywhere, Category = "Misc Variables")
		FVector GoalDestination;
	UPROPERTY(EditAnywhere, Category = "Misc Variables")
		FRotator HomeRotation;
	UPROPERTY(EditAnywhere, Category = "Misc Variables")
		FRotator GoalRotation;

private:
	int32 owningPlayerID;
	bool bIsGateOpen;

public:
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* UnitBase;
	UPROPERTY(EditAnywhere, Category = "Components")
		UTextRenderComponent* NameText;
	UPROPERTY(EditAnywhere, Category = "Components")
		UTextRenderComponent* UnitTypeText;
	UPROPERTY(EditAnywhere, Category = "Components")
		UTextRenderComponent* DescriptionText;
	UPROPERTY(EditAnywhere, Category = "Components")
		UTextRenderComponent* AttackText;
	UPROPERTY(EditAnywhere, Category = "Components")
		UTextRenderComponent* HealthText;
	UPROPERTY(EditAnywhere, Category = "Components")
		UTextRenderComponent* CostText;

	UFUNCTION(BlueprintPure)
		int32 GetOwningPlayerID() { return owningPlayerID; };
	UFUNCTION(BlueprintCallable)
		void SetOwningPlayerID(int32 value) { owningPlayerID = value; };

public:
	// Sets default values for this actor's properties
	ABoardUnit();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TakesDamage(ABoardUnit* dealingUnit, int32 damageAttack, int32 damageHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnActivePlayerTurn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnEndActivePlayerTurn();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float deltaTime) override;

private:
	UFUNCTION()
		void RemoveUnitActor();
	void RemoveUnitActor_Delayed();

	UFUNCTION()
		void SetUnitHiddenState(bool newHidden);

	UFUNCTION()
		void ServerSetUnitData(FName cardName, ECardSet unitSet, FCardData dataStruct, bool setUnitDataFromStruct);

	UFUNCTION()
		void MulticastSetUnitVisuals();
	UFUNCTION()
		void MulticastUpdateVisualStats();

	UFUNCTION()
		void ForceMoveCardDirectlyToGraveyard(AGraveyard* graveyard);

	UFUNCTION()
		void OpenGate();
	UFUNCTION()
		void CloseGate();

	void SetUnitData(FName cardName, ECardSet cardSet, FCardData dataStruct, bool setUnitDataFromStruct);
	void SetUnitVisuals();

	void GenerateDescription(FString descriptionText);

	void PlayMovementAnimation(E3DCardMovementState cardMovementState, FVector manualGoalDestination, FRotator manualGoalRotator, AActor* orGoalActor, float zOffset, float errorTolerance, float interSpeed);

	FRotator InterpSelfToRotation(bool& arrived);
	FVector InterpSelfToLocation(bool& arrived);

	void ArrivedAtDestination();
};
