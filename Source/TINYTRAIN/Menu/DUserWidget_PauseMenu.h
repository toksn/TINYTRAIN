// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DUserWidget_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class TINYTRAIN_API UDUserWidget_PauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonResume();
	/*
	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonSettings();
	*/
	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonRestart();

	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonMainMenu();
	
	
};
