// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWHUD.h"

#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"

ATCWHUD::ATCWHUD(const FObjectInitializer& ObjectInitializer) : AHUD(ObjectInitializer)
{
	//Pre-23
}

void ATCWHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATCWHUD::BeginPlay()
{

}

void ATCWHUD::DrawHUD()
{
	Super::DrawHUD();
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	int viewportX, viewportY;
	pc->GetViewportSize(viewportX, viewportY);
	int sizeX = Canvas->SizeX - viewportX;
	int sizeY = Canvas->SizeY - viewportY;

	float cursorX, cursorY;
	pc->GetMousePosition(cursorX, cursorY);
	float locationX = cursorX + ((float)sizeX / 2.0f);
	float locationY = cursorY + ((float)sizeY / 2.0f);


	DrawMaterial(GetCurrentCursorMaterial(), locationX - (CursorSize / 2.0f), locationY - (CursorSize / 2.0f), CursorSize, CursorSize, 0.0f, 0.0f, 0.0f, 0.0f);
}

UMaterial* ATCWHUD::GetCurrentCursorMaterial()
{
	return nullptr;
}
