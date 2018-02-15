// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINGameState.h"
#include "DUserWidget_PauseMenu.h"

void UDUserWidget_PauseMenu::ButtonResume()
{
	// deactivate pause only when in pausemenu
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate && gamestate->GetMenuState() == ATINYTRAINGameState::TINYTRAIN_MENUSTATE::PAUSE)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::NONE);
		gamestate->SetPrevGameState();
	}
}
/*
void UDUserWidget_PauseMenu::ButtonSettings()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		// call settings menu
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::SETTINGS);
	}

}
*/
void UDUserWidget_PauseMenu::ButtonRestart()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		gamestate->StartLevel();
	}
}


void UDUserWidget_PauseMenu::ButtonMainMenu()
{
	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		//static ConstructorHelpers::FClassFinder<UUserWidget> mainMenu(TEXT("/Game/MyContent/HUD/DMainMenu.DMainMenu_C"));
		//StartingWidgetClass = mainMenu.Class;

		//UClass* mainMenuWidget = StaticLoadClass(UUserWidget::StaticClass(), NULL, TEXT("/Game/MyContent/HUD/DMainMenu.DMainMenu_C"));

		//UUserWidget* Result = FindObject<UUserWidget>(, TEXT("/Game/MyContent/HUD/DMainMenu.DMainMenu"));
		//static ConstructorHelpers::FClassFinder<UUserWidget> mainMenu(TEXT("/Game/MyContent/HUD/DMainMenu.DMainMenu_C"));
		//StartingWidgetClass = mainMenu.Class;

		// call main menu
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::MAIN);
	}
}
