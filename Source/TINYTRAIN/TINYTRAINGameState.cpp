// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINGameState.h"
#include "TINYTRAINPawn.h"
#include "TINYTRAINHud.h"
#include "TrainVehicle.h"
#include "RailTrack.h"
#include "Sound/SoundCue.h"
//#include "DStone.h"
//#include "DStoneGrid.h"
#include "Menu/DUserWidget_PostGameMenu.h"
//#include "DUserWidget_SettingsMenu.h"
//#include "DRenderedTextActor.h"
//#include "diamontestGameMode.h"


ATINYTRAINGameState::ATINYTRAINGameState() : Super()
{
	//mMatchesCount_byColor = new int32[ADStone::STONE_COLOR::COUNT];
	
	static ConstructorHelpers::FClassFinder<UUserWidget> mainMenu(TEXT("/Game/UsedAssets/HUD/TTMainMenu.TTMainMenu_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> pauseMenu(TEXT("/Game/UsedAssets/HUD/TTPauseMenu.TTPauseMenu_C"));
	//static ConstructorHelpers::FClassFinder<UUserWidget> settingsMenu(TEXT("/Game/UsedAssets/HUD/TTSettingsMenu.TTSettingsMenu_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> postGameMenu(TEXT("/Game/UsedAssets/HUD/TTPostGameMenu.TTPostGameMenu_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> hud(TEXT("/Game/UsedAssets/HUD/TTHUD.TTHUD_C"));
	//static ConstructorHelpers::FClassFinder<UUserWidget> creditsScreen(TEXT("/Game/UsedAssets/HUD/TTCredits.TTCredits_C"));
	//static ConstructorHelpers::FClassFinder<UUserWidget> howtoplayScreen(TEXT("/Game/UsedAssets/HUD/TTHowToPlay.TTHowToPlay_C"));

	//static ConstructorHelpers::FObjectFinder<UFont>font(TEXT("/Game/MyContent/Font/cour"));

	//static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader_minsound(TEXT("SoundCue'/Game/UsedAssets/Sounds/SuccessCue.SuccessCue'"));
	//static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader_maxsound(TEXT("SoundCueSoundCue'/Game/UsedAssets/Sounds/goodSuccessCue.goodSuccessCue'"));
	//static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader_bgm01(TEXT("SoundCueSoundCue'/Game/UsedAssets/Sounds/BGM03_ukulele_Cue.BGM03_ukulele_Cue'"));
	//static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader_bgm02(TEXT("SoundCueSoundCue'/Game/UsedAssets/Sounds/BGM02_cute_Cue.BGM02_cute_Cue'"));
	
	mSound_Success_Min = NULL;		//soundCueLoader_minsound.Object;
	mSound_Success_Max = NULL;		//soundCueLoader_maxsound.Object;
	mSound_BGM_1 =		 NULL;		//	soundCueLoader_bgm01.Object;
	mSound_BGM_2 =		 NULL;		//	soundCueLoader_bgm02.Object;
	
	//mFont = font.Object;
	
	MainMenuClass = mainMenu.Class;
	PauseMenuClass = pauseMenu.Class;
	GameOverScreen = postGameMenu.Class;
	hudClass = hud.Class;
	hudWidget = NULL;

	//SettingsMenuClass = settingsMenu.Class;
	//CreditsScreen = creditsScreen.Class;
	//HowToPlayScreen = howtoplayScreen.Class;
	StartingWidgetClass = MainMenuClass;

	PrimaryActorTick.bCanEverTick = true;

	Reset();
	mCurrentMenuState = ATINYTRAINGameState::TINYTRAIN_MENUSTATE::NONE;
	mCurrentGameState = ATINYTRAINGameState::TINYTRAIN_GAMESTATE::IDLE;
	mCurrentLevelState = TINYTRAIN_LEVELSTATE::NOLEVEL;
}

ATINYTRAINGameState::~ATINYTRAINGameState()
{
	delete[] mMatchesCount_byColor;
}

void ATINYTRAINGameState::BeginPlay()
{
	Super::BeginPlay();

	// init volume from settings of gamemode
	/*AdiamontestGameMode* gm = Cast<AdiamontestGameMode>(GetDefaultGameMode());
	if (gm && gm->IsA<AdiamontestGameMode>())
	{
		FDiamontGameSettings settings = gm->GetGameSettings();
		SetMusicVolume(settings.mVolume_Music);
		SetEffectVolume(settings.mVolume_Effect);
	}*/

	//if (FMath::RandBool())
	//	UGameplayStatics::PlaySound2D(GetWorld(), mSound_BGM_1, mSound_BGM_1->GetVolumeMultiplier());
	//else
	//	UGameplayStatics::PlaySound2D(GetWorld(), mSound_BGM_2, mSound_BGM_2->GetVolumeMultiplier());

	hudWidget = CreateWidget<UUserWidget>(GetWorld(), hudClass);
	hudWidget->SetVisibility(ESlateVisibility::Collapsed);
	hudWidget->AddToViewport();	
		
	ChangeMenuWidget(StartingWidgetClass);
	ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::MAIN);
	SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::MENU);
}

void ATINYTRAINGameState::Reset()
{
	Super::Reset();

	mScore = 0;
	mMatchesCount = 0;
	//for (int32 i = 0; i < ADStone::STONE_COLOR::COUNT; i++)
	//	mMatchesCount_byColor[i] = 0;

	//// get rules by gamemode
	//AdiamontestGameMode* gm = Cast<AdiamontestGameMode>(GetDefaultGameMode());
	//if (gm && gm->IsA<AdiamontestGameMode>())
	//{
	//	FDiamontGameSettings settings = gm->GetGameSettings();
	//
	//	mGameTime = settings.mGameTime;
	//	mTimeToAdd = settings.mAdditionalTimeOnLongMatch;
	//	if (mGameTime < 0.0f)
	//		mGameTime = 0.0f;
	//}	
}

// Called every frame
void ATINYTRAINGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// decrease gametime
	if(mCurrentGameState != TINYTRAIN_GAMESTATE::MENU)
		mGameTime -= DeltaTime;

	// end of game 
	if (mGameTime < 0.0f)
		mGameTime = 0.0f;		
}

void ATINYTRAINGameState::StartLevel()
{
	// todo:
	// open next (selected) level



	// iterate the active pawns and start their trainssssss
	auto pawnIt = GetWorld()->GetPawnIterator(); 
	ATINYTRAINPawn* pawn = NULL;
	while (pawnIt && (pawn = Cast<ATINYTRAINPawn>(pawnIt->Get()))!=NULL)
	{
		if (pawn->m_trainVehicle)
		{
			pawn->m_trainVehicle->Reset();
			pawn->m_trainVehicle->Play();
		}	

		if (pawn->m_RailTrackToExtend)
			pawn->m_RailTrackToExtend->Reset();

		pawn->MoveCameraToLastRail();

		++pawnIt;
	}

	/*
	auto playerIt = GetWorld()->GetPlayerControllerIterator();
	APlayerController* player = NULL;
	while (playerIt && (player = Cast<APlayerController>(playerIt->Get())) != NULL)
	{
		player->RestartLevel();
		
		++playerIt;
	}
	*/

	// unpause (in case we came from pause menu)
	//if (mPrevMenuState == ATINYTRAINGameState::TINYTRAIN_MENUSTATE::PAUSE)
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// reset and unset menu
	Reset();
	ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::NONE);

	// start game -> idle
	SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::IDLE);

	// todo: countdown
	SetLevelState(TINYTRAIN_LEVELSTATE::RUNNING);
}

// end current level with given levelstate and go to post game menu
void ATINYTRAINGameState::EndLevel(TINYTRAIN_LEVELSTATE levelstate)
{
	if (mCurrentLevelState != TINYTRAIN_LEVELSTATE::NOLEVEL)
	{
		SetLevelState(levelstate);
		SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::MENU);
		ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::GAMEOVER);
	}
}

// todo: not used anymore?
bool ATINYTRAINGameState::CheckForGameOver()
{
	bool bGameEnd = false;
	
	/*
	// end by time. 
	if (mGameTime <= 0.0f)
	{
		// change to gameover-screen
		ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::GAMEOVER);
		SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::MENU);
		bGameEnd = true;
	}*/
	
	return bGameEnd;
}

void ATINYTRAINGameState::ChangeGameMenu(TINYTRAIN_MENUSTATE menu)
{
	if (mCurrentMenuState != menu)
	{
		switch (menu)
		{
		case TINYTRAIN_MENUSTATE::MAIN:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(MainMenuClass);
			break;
		case TINYTRAIN_MENUSTATE::PAUSE:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(PauseMenuClass);
			break;
		case TINYTRAIN_MENUSTATE::SETTINGS:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(SettingsMenuClass);
			break;
		case TINYTRAIN_MENUSTATE::GAMEOVER:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(GameOverScreen);
			break;
		case TINYTRAIN_MENUSTATE::CREDITS:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(CreditsScreen);
			break;
		case TINYTRAIN_MENUSTATE::HOWTOPLAY:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(HowToPlayScreen);
			break;
		case TINYTRAIN_MENUSTATE::NONE:
			mPrevMenuState = mCurrentMenuState;
			mCurrentMenuState = menu;
			ChangeMenuWidget(NULL);
			break;
		}
#ifdef _DEBUG
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("menustate changed: From %i to %i"), (int32)mPrevMenuState, (int32)mCurrentMenuState));
#endif
	}
}

void ATINYTRAINGameState::ChangeToPrevMenu()
{
	if (mPrevMenuState < 0)
		mPrevMenuState = TINYTRAIN_MENUSTATE::MAIN;
	// close current and switch to previous menu
	ChangeGameMenu(mPrevMenuState);
}

void ATINYTRAINGameState::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::MENU);

			CurrentWidget->AddToViewport();
		}
	}
}

ATINYTRAINGameState::TINYTRAIN_MENUSTATE ATINYTRAINGameState::GetMenuState()
{
	return mCurrentMenuState;
}
ATINYTRAINGameState::TINYTRAIN_MENUSTATE ATINYTRAINGameState::GetPrevMenuState()
{
	return mPrevMenuState;
}

ATINYTRAINGameState::TINYTRAIN_GAMESTATE ATINYTRAINGameState::GetGameState()
{
	return mCurrentGameState;
}
ATINYTRAINGameState::TINYTRAIN_GAMESTATE ATINYTRAINGameState::GetPrevGameState()
{
	return mPrevGameState;
}

void ATINYTRAINGameState::SetLevelState(TINYTRAIN_LEVELSTATE ls)
{
	if (ls != mCurrentLevelState)
	{
#ifdef _DEBUG
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("levelstate changed: From %i to %i"), (int32)mCurrentLevelState, (int32)ls));
#endif

		mCurrentLevelState = ls;
	}
}

TINYTRAIN_LEVELSTATE ATINYTRAINGameState::GetLevelState()
{
	return mCurrentLevelState;
}

void ATINYTRAINGameState::SetGameState(TINYTRAIN_GAMESTATE gs)
{
	if (gs != mCurrentGameState)
	{
		// turn HUD on/off
		auto playerIt = GetWorld()->GetPlayerControllerIterator();
		APlayerController* player = NULL;
		bool bShowHUD = gs != ATINYTRAINGameState::TINYTRAIN_GAMESTATE::MENU;

		if (bShowHUD)
			hudWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		else if(!bShowHUD )
			hudWidget->SetVisibility(ESlateVisibility::Collapsed);

		while (playerIt && (player = Cast<APlayerController>(playerIt->Get())) != NULL)
		{
			ATINYTRAINHud* hud = Cast<ATINYTRAINHud>(player->GetHUD());
			if(hud)
				hud->m_bShowLifetimeLines = bShowHUD;

			++playerIt;
		}

		mPrevGameState = mCurrentGameState;
		mCurrentGameState = gs;
#ifdef _DEBUG
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("gamestate changed: From %i to %i"), (int32)mPrevGameState, (int32)gs));
#endif
	}
}
void ATINYTRAINGameState::SetPrevGameState()
{
	SetGameState(mPrevGameState);
}

int32 ATINYTRAINGameState::GetScore()
{
	return mScore;
}

void ATINYTRAINGameState::SetScore(int32 newScore)
{
	mScore = newScore;
}
void ATINYTRAINGameState::AddScore(int32 addScore)
{
	SetScore(mScore + addScore);
}

void ATINYTRAINGameState::SetMusicVolume(float fVolume)
{
	if (mSound_BGM_1 && mSound_BGM_2)
	{
		mSound_BGM_1->VolumeMultiplier = 0.25f * fVolume;
		mSound_BGM_2->VolumeMultiplier = 0.25f * fVolume;
	}
}

void ATINYTRAINGameState::SetEffectVolume(float fVolume)
{
	if (mSound_Success_Min && mSound_Success_Max)
	{
		mSound_Success_Min->VolumeMultiplier = 0.35f * fVolume;
		mSound_Success_Max->VolumeMultiplier = 0.35f * fVolume;
	}
}

float ATINYTRAINGameState::GetRemainingGameTime()
{
	return mGameTime;
}

bool ATINYTRAINGameState::CallCurrentMenuAccept()
{
	UDUserWidget_PostGameMenu* gameover = NULL;
	//UDUserWidget_SettingsMenu* settings = NULL;
	switch (mCurrentMenuState)
	{
	case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::MAIN:
		StartLevel();
		return true;
	case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::GAMEOVER:
		gameover = Cast<UDUserWidget_PostGameMenu>(CurrentWidget);
		if(gameover)
			gameover->ButtonOK();
		return true;
	//case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::SETTINGS:
	//	settings = Cast<UDUserWidget_SettingsMenu>(CurrentWidget);
	//	if(settings)
	//		settings->ButtonOK();
	//	return true;
	default:
		break;
	}

	return false;
}

bool ATINYTRAINGameState::CallCurrentMenuDecline()
{
	switch (mCurrentMenuState)
	{
	case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::MAIN:
		// QUIT();
		break;
	case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::SETTINGS:
		ChangeToPrevMenu();
		return true;
	// ACTIVATE PAUSE
	case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::NONE:
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::PAUSE);
		SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::MENU);
		return true;
	// DEACTIVATE PAUSE
	case ATINYTRAINGameState::TINYTRAIN_MENUSTATE::PAUSE:
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::NONE);
		SetPrevGameState();
		return true;
	}
	return false;
}

