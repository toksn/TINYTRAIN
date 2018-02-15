// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DUserWidget_MainMenu.generated.h"


class ATINYTRAINGameState;
/**
 * 
 */
UCLASS()
class TINYTRAIN_API UDUserWidget_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonStartNewGame();

	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonSettings();

	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonQuit();

	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
		void ButtonCredits();

	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
		void ButtonHowToPlay();

private:
	//ATINYTRAINGameState* GetGameState();

};
