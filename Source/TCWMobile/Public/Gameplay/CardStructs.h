// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "Engine.h"

#include "Enums.h"

#include "CardStructs.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardVisuals_Struct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Frame")
        UTexture2D* FrameTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Image")
        UTexture2D* ImageTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Backside")
        UTexture2D* BackTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Frame")
        UMaterialInstance* FrameMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Image")
        UMaterialInstance* ImageMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Backside")
        UMaterialInstance* BackMaterial;
};

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardAttack_Struct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int ManaCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
        bool CanAttackPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
        bool CanAttackCards;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    //    EDamageType DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
        UParticleSystem* AttackParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
        USoundCue* AttackSoundCue;
};

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardHealth_Struct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int LifeExpectancy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
        UParticleSystem* DeathParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
        USoundCue* DeathSoundCue;
};

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardAbility_Struct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        EAbilityType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int AbilityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        EAbilityTrigger Trigger;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
    //    ECardUseState AfterUseState;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
    //    EPlayers AffectingPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
        UParticleSystem* Particle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
        USoundCue* SoundCue;
};

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardDescription_Struct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description")
        FString Description;
};

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardPlacement_Struct
{
    GENERATED_BODY()

public:
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
    //    EPlayers Owner;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int Cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
        UParticleSystem* DeployParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
        USoundCue* DeploySoundCue;
};

/**
 *
 */
USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardSettings_Struct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        bool Unlocked;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        bool AddToPlayerDeck;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int MaxNumInDeck;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        int Weight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
        FVector2D ScreenSize;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FText Name;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		ECardType Type;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
        ECardRarity Rarity;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
        ECardColor Color;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		bool Special;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FCardVisuals_Struct Visual;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FCardAttack_Struct Attack;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FCardHealth_Struct Health;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		TArray<FCardAbility_Struct> Abilities;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FCardDescription_Struct Description;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FCardPlacement_Struct PlacementSettings;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
		FCardSettings_Struct DeckSettings;

	//UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Card Data")
	//	FCardSystemData_Struct CardSystemData;
};

USTRUCT(BlueprintType, Atomic, Category = "Card Data")
struct FCardsInDeck
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prebuild Deck Data")
        FName CardName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prebuild Deck Data")
        int32 Quantity;
};

USTRUCT(BlueprintType)
struct FPrebuildDeck : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Prebuild Deck Data")
        FText DeckName;
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Prebuild Deck Data")
        bool bEditable;
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Prebuild Deck Data")
        FString DeckDescription;
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Prebuild Deck Data")
        TArray<FCardsInDeck> CardsInDeck;
};

USTRUCT(BlueprintType, Atomic)
struct FPlayerDeck
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Saved Deck Data")
        FText DeckName;
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Saved Deck Data")
        bool bEditable;
    UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Saved Deck Data")
        TArray<FCardsInDeck> CardsInDeck;
};
