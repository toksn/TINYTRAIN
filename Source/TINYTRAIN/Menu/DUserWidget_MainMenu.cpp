// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINGameState.h"
#include "DUserWidget_MainMenu.h"

void UDUserWidget_MainMenu::ButtonStartNewGame()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		gamestate->StartLevel();
	}
}

void UDUserWidget_MainMenu::ButtonSettings()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		// call settings menu
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::SETTINGS);
	}
}

void UDUserWidget_MainMenu::ButtonCredits()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		// call credits menu
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::CREDITS);
	}
}

void UDUserWidget_MainMenu::ButtonHowToPlay()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		// call howtoplay menu
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::HOWTOPLAY);
	}
}

void UDUserWidget_MainMenu::ButtonQuit()
{
	// shut down the whole game
	FGenericPlatformMisc::RequestExit(false);
}
