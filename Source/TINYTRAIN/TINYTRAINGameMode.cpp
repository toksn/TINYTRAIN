// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TINYTRAIN.h"
#include "TINYTRAINGameMode.h"
#include "TINYTRAINPlayerController.h"
#include "TINYTRAINPawn.h"
#include "TINYTRAINHud.h"
#include "TINYTRAINGameState.h"

ATINYTRAINGameMode::ATINYTRAINGameMode()
{
	// no pawn by default
	DefaultPawnClass = ATINYTRAINPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ATINYTRAINPlayerController::StaticClass();

	HUDClass = ATINYTRAINHud::StaticClass();

	GameStateClass = ATINYTRAINGameState::StaticClass();
}
