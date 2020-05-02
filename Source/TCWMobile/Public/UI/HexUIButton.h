// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "HexUIButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHexUIButtonSignature);

/**
 *
 */
UCLASS(Abstract)
class TCWMOBILE_API UHexUIButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UHexUIButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System")
	UTexture2D* ButtonTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System")
	UTexture2D* IconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* HexUIButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HexUIIcon;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FHexUIButtonSignature OnClicked;

private:
	FScriptDelegate OnHexUIButtonClicked;

	UFUNCTION()
	void OnClickedInternal() { if (OnClicked.IsBound()) { OnClicked.Broadcast(); } };
};
