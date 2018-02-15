// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DUserWidget_PostGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class TINYTRAIN_API UDUserWidget_PostGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	//UFUNCTION(BlueprintCallable, Category = "GameOverWidgetVariables")
	//int32 GetScore();

	UFUNCTION(BlueprintCallable, Category = "GameOverWidgetVariables")
	void ButtonOK();
	
	UFUNCTION(BlueprintCallable, Category = "SettingsProcessing")
	void OnInit();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameOverWidgetVariables")
	int32 mScore;

	// HIGHSCORE STUFF
	
	//UFUNCTION(BlueprintCallable, Category = "SettingsProcessing")
	//void OnChangeName(FText name);
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameOverWidgetVariables")
	//FString mPlayerName;
	//UPROPERTY()
	//class UDHighScoreSave* mHighScoreSave;
	//FString mHighScoreFileName;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameOverWidgetVariables")
	//bool mReachedHighScore;
};
