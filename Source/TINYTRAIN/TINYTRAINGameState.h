// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "GameFramework/GameState.h"
#include "TINYTRAINGameState.generated.h"

UENUM(BlueprintType)
enum TINYTRAIN_LEVELSTATE
{
	NOLEVEL,
	COUNTDOWN,
	RUNNING,
	FAILED,
	WON
};

/**
 * 
 */
UCLASS()
class TINYTRAIN_API ATINYTRAINGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	enum TINYTRAIN_GAMESTATE {
		IDLE,
		DRAWING,
		MENU
	};

	

	enum TINYTRAIN_MENUSTATE {
		NONE,
		MAIN,
		PAUSE,
		SETTINGS,
		GAMEOVER,

		CREDITS,
		HOWTOPLAY
	};

	ATINYTRAINGameState();
	~ATINYTRAINGameState();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Reset() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartLevel();

	virtual void EndLevel(TINYTRAIN_LEVELSTATE levelstate);

	//ADStoneGrid * RespawnStoneGrid();

	bool CheckForGameOver();

	void ChangeGameMenu(TINYTRAIN_MENUSTATE menu);
	void ChangeToPrevMenu();

	TINYTRAIN_MENUSTATE GetMenuState();
	TINYTRAIN_MENUSTATE GetPrevMenuState();
	
	TINYTRAIN_GAMESTATE GetPrevGameState();
	TINYTRAIN_GAMESTATE GetGameState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	TINYTRAIN_LEVELSTATE GetLevelState();

	void SetLevelState(TINYTRAIN_LEVELSTATE ls);

	void SetGameState(TINYTRAIN_GAMESTATE gs);
	void SetPrevGameState();

	int32 GetScore();
	void SetScore(int32 newScore);
	void AddScore(int32 addScore);

	//bool ProcessColorMatches(TArray<class ADStone*> stones);

	//void SetGridStaticStonesProperty(bool bStatic);
	void SetMusicVolume(float fVolume);
	void SetEffectVolume(float fVolume);
	//ADStoneGrid * GetCurrentGrid();
	//bool GetGridStaticStonesProperty();

	float GetRemainingGameTime();

	bool CallCurrentMenuAccept();
	bool CallCurrentMenuDecline();
	
	//bool mMinEffectBlocked;
	//bool mMaxEffectBlocked;

protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;
	
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> PauseMenuClass;
	TSubclassOf<UUserWidget> SettingsMenuClass;
	TSubclassOf<UUserWidget> hudClass;
	TSubclassOf<UUserWidget> GameOverScreen;
	TSubclassOf<UUserWidget> CreditsScreen;
	TSubclassOf<UUserWidget> HowToPlayScreen;

	UUserWidget* hudWidget;

	//UPROPERTY()
	//UFont* mFont;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentWidget;

	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
private:
	int32 mScore;
	float mGameTime;
	float mTimeToAdd;
	int32 mMatchesCount;
	int32* mMatchesCount_byColor;
	FTimerHandle mGameTimerHandle;
	//ADStoneGrid* mCurrentGrid;

	UPROPERTY() class USoundCue* mSound_BGM_1;
	UPROPERTY() class USoundCue* mSound_BGM_2;
	UPROPERTY()	class USoundCue* mSound_Success_Min;
	UPROPERTY() class USoundCue* mSound_Success_Max;

	TINYTRAIN_GAMESTATE mCurrentGameState, mPrevGameState;	
	TINYTRAIN_MENUSTATE mCurrentMenuState, mPrevMenuState;
	TINYTRAIN_LEVELSTATE mCurrentLevelState;
};
