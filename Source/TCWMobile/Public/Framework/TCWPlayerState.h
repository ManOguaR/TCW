// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "Gameplay/PlayerState/AccountProfileData.h"

#include "TCWPlayerState.generated.h"

class ATCWGameMode;
class ATCWPlayerController;

/**
 *
 */
UCLASS()
class TCWMOBILE_API ATCWPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATCWPlayerState(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
		int32 GamePlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ATCWPlayerController* OwningPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ATCWGameMode* GameModeRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana)
		int32 Mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana_Max)
		int32 Mana_Max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
		int32 Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health_Max)
		int32 Health_Max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CardsInHand)
		int32 CardsInHand;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CardsInDeck)
		int32 CardsInDeck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_ActiveCards)
		int32 ActiveCards;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		int32 MaxCardsInDeck;

	int32 PlayerTurn;

private:
	UAccountProfileData* AccountProfile;

public:
	UFUNCTION(BlueprintCallable)
		void InitializeAttributes();
	UFUNCTION(BlueprintCallable)
		void InitializeMana();
	UFUNCTION(BlueprintCallable)
		void InitializeHealth();

private:
	UFUNCTION()
		void OnRep_Mana();
	UFUNCTION()
		void OnRep_Mana_Max();
	UFUNCTION()
		void OnRep_Health();
	UFUNCTION()
		void OnRep_Health_Max();
	UFUNCTION()
		void OnRep_CardsInHand();
	UFUNCTION()
		void OnRep_CardsInDeck();
	UFUNCTION()
		void OnRep_ActiveCards();

public:
	void UpdatePlayerCardsStates(int32 cardInHand, int32 cardInDeck, int32 activeCards);

private:
	void UpdateUI();

	void InitializeAccountProfile();

	void BeginPlay_Delayed();
};
