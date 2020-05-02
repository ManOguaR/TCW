// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWPlayerState.h"
#include "TCWGameMode.h"
#include "TCWPlayerController.h"
#include "MiscFunctionLibrary.h"

#include "Kismet\GameplayStatics.h"
#include "Net\UnrealNetwork.h"

ATCWPlayerState::ATCWPlayerState(const FObjectInitializer& ObjectInitializer) : APlayerState(ObjectInitializer)
{
	//Pre-26
	//EXE-8
}

void ATCWPlayerState::BeginPlay()
{
	//EXE-5
	UKismetSystemLibrary::Delay(this, 0.5f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.5f);

	GameModeRef = Cast<ATCWGameMode>(UGameplayStatics::GetGameMode(this));

	InitializeAttributes();
}

void ATCWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ATCWPlayerState, Mana);
	DOREPLIFETIME(ATCWPlayerState, Mana_Max);
	DOREPLIFETIME(ATCWPlayerState, Health);
	DOREPLIFETIME(ATCWPlayerState, Health_Max);
	DOREPLIFETIME(ATCWPlayerState, CardsInHand);
	DOREPLIFETIME(ATCWPlayerState, CardsInDeck);
	DOREPLIFETIME(ATCWPlayerState, ActiveCards);
}

void ATCWPlayerState::InitializeAttributes()
{
	if (HasAuthority())
	{
		InitializeMana();
		InitializeHealth();
		InitializeAccountProfile();
	}
}

void ATCWPlayerState::InitializeMana()
{
	if (HasAuthority())
	{
		Mana = 0;
		//OnRep_Mana();
	}
}

void ATCWPlayerState::InitializeHealth()
{
	if (HasAuthority())
	{
		Health = GameModeRef->PlayerStartingHealth;
		//OnRep_Health();
	}
}

void ATCWPlayerState::OnRep_Mana()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::OnRep_Mana_Max()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::OnRep_Health()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::OnRep_Health_Max()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::OnRep_CardsInHand()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::OnRep_CardsInDeck()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::OnRep_ActiveCards()
{
	UpdateUI();
	//TODO: NEXT
}

void ATCWPlayerState::UpdatePlayerCardsStates(int32 cardInHand, int32 cardInDeck, int32 activeCards)
{
	if (HasAuthority())
	{
		CardsInHand = cardInHand;
		//OnRep_CardsInHand();
		CardsInDeck = cardInDeck;
		OnRep_CardsInDeck();
		ActiveCards = activeCards;
		OnRep_ActiveCards();
	}
}

void ATCWPlayerState::UpdateUI()
{
	ATCWPlayerController* controller = Cast<ATCWPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (controller->IsValidLowLevel())
	{

	}
}

void ATCWPlayerState::InitializeAccountProfile()
{
	AccountProfile = NewObject<UAccountProfileData>();
	//AccountProfile = NewObject<UAccountProfileData>(this, TEXT("AccountProfile"));
}
