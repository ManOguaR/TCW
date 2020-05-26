// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWSaveGame.h"

UTCWSaveGame::UTCWSaveGame(const FObjectInitializer& ObjectInitializer) : USaveGame(ObjectInitializer)
{

}

TArray<FString> UTCWSaveGame::GetDeckList()
{
	return DeckList;
}
