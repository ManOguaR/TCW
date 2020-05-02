// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "HexUIButton.h"
#include "SlateCore.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetTree.h"

UHexUIButton::UHexUIButton(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	//PRE - 13
	OnHexUIButtonClicked.BindUFunction(this, "OnClickedInternal");
}

void UHexUIButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (HexUIButton->IsValidLowLevelFast())
	{
		FButtonStyle newStyle = FButtonStyle();

		//FSlateDynamicImageBrush baseBrush = FSlateDynamicImageBrush(ButtonTexture, FVector2D(138, 120), ButtonTexture->GetFName());

		FSlateImageBrush baseBrush = FSlateImageBrush(ButtonTexture, FVector2D(138, 120));// , ButtonTexture->GetFName());

		newStyle.Normal = FSlateImageBrush(baseBrush);
		newStyle.Normal.DrawAs = ESlateBrushDrawType::Box;
		newStyle.Normal.Margin = FMargin(0.25f);
		newStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

		newStyle.Pressed = FSlateImageBrush(baseBrush);
		newStyle.Pressed.DrawAs = ESlateBrushDrawType::Box;
		newStyle.Pressed.Margin = FMargin(0.25f);
		newStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.2f, 0.2f, 1.0f, 1.0f));

		newStyle.Hovered = FSlateImageBrush(baseBrush);
		newStyle.Hovered.DrawAs = ESlateBrushDrawType::Box;
		newStyle.Hovered.Margin = FMargin(0.25f);
		newStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.33f, 0.33f, 0.33f, 1.0f));

		newStyle.NormalPadding = FMargin(2.0f);
		newStyle.PressedPadding = FMargin(2.0f, 3.0f, 2.0f, 1.0f);

		HexUIButton->SetStyle(newStyle);

		HexUIButton->OnClicked.Clear();
		HexUIButton->OnClicked.AddUnique(OnHexUIButtonClicked);
	}
	if (HexUIIcon->IsValidLowLevelFast())
	{
		HexUIIcon->SetBrushFromTexture(IconTexture, true);
	}
}

void UHexUIButton::NativeConstruct()
{
	Super::NativeConstruct();
}