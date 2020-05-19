// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameProfileUI.generated.h"

class ATCWPlayerState;
class UResourcesBar;

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePlayerUI, ATCWPlayerState*, playerStateRef);

/**
 *
 */
UCLASS()
class UGameProfileUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameProfileUI(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintCallable, Category = "Events")
		FUpdatePlayerUI OnUpdatePlayerUI;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Player_Image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* CardsInDeck_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Mana_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayerHealth_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayerTurn_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayerName_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UResourcesBar* ResourcesBar;

private:
	ATCWPlayerState* PlayerStateRef;

private:
	UFUNCTION()
		void UpdatePlayerUI(ATCWPlayerState* playerStateRef);
};
