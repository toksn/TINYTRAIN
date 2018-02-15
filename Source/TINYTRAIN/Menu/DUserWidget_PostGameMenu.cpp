// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINGameState.h"
//#include "TINYTRAINGameMode.h"


//#include "DHighScoreSave.h"
#include "DUserWidget_PostGameMenu.h"



void UDUserWidget_PostGameMenu::ButtonOK()
{
	/*
	// highscores were loaded correctly 
	if (mHighScoreSave)
	{
		//int32 highscoreEntries = mHighScoreSave->GetHighScores().Num();
		//// either less than 10 entries or the newScore is within the topten
		//mReachedHighScore = highscoreEntries < 10 || mHighScoreSave->GetHighScores()[highscoreEntries - 10].mScore < mScore;

		// highscore top10 has been reached
		if (mReachedHighScore)
		{
			// ADD AND SAVE HIGHSCORE
			mHighScoreSave->AddToHighScore(mScore, mPlayerName);
			UGameplayStatics::SaveGameToSlot(mHighScoreSave, mHighScoreFileName, 0);
		}
	}*/

	ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState());
	if (gamestate)
	{
		// call main menu
		gamestate->ChangeGameMenu(ATINYTRAINGameState::TINYTRAIN_MENUSTATE::MAIN);
	}
}

void UDUserWidget_PostGameMenu::OnInit()
{
	//mHighScoreSave = NULL;
	/*
	if (ATINYTRAINGameState* gamestate = Cast<ATINYTRAINGameState>(GetWorld()->GetGameState()))
	{
		mScore = gamestate->GetScore();

		AdiamontestGameMode* gamemode = Cast<AdiamontestGameMode>(gamestate->GetDefaultGameMode());
		if (gamemode)
		{
			// get game settings to find the correct highscore filename
			FDiamontGameSettings settings = gamemode->GetGameSettings();
			mHighScoreFileName = FString::Printf(TEXT("hs_%i_%i_%i_%i_%i"), settings.mGridSize.X * settings.mGridSize.Y, (int32)settings.mGameTime, settings.mMinStonesForMatch, settings.mScoreForMatch, (int32)settings.mAdditionalTimeOnLongMatch);

			// load highscores for current settings
			mHighScoreSave = Cast<UDHighScoreSave>(UGameplayStatics::LoadGameFromSlot(mHighScoreFileName, 0));

			// check settings against loaded highscore settings
			bool bCorrectSettings = false;
			if (mHighScoreSave)
			{
				bCorrectSettings = mHighScoreSave->mBonusTime == settings.mAdditionalTimeOnLongMatch &&
					mHighScoreSave->mGameTime == settings.mGameTime &&
					mHighScoreSave->mMinMatchStones == settings.mMinStonesForMatch &&
					mHighScoreSave->mMinMatchScore == settings.mScoreForMatch &&
					mHighScoreSave->mStonesCount == settings.mGridSize.X * settings.mGridSize.Y;
			}

			// no file found or settings dont match -> create new savegame object to be able to save highscores
			if (!mHighScoreSave || !bCorrectSettings)
			{
				mHighScoreSave = Cast<UDHighScoreSave>(UGameplayStatics::CreateSaveGameObject(UDHighScoreSave::StaticClass()));

				mHighScoreSave->mBonusTime = settings.mAdditionalTimeOnLongMatch;
				mHighScoreSave->mGameTime = settings.mGameTime;
				mHighScoreSave->mMinMatchStones = settings.mMinStonesForMatch;
				mHighScoreSave->mMinMatchScore = settings.mScoreForMatch;
				mHighScoreSave->mStonesCount = settings.mGridSize.X * settings.mGridSize.Y;
			}

			// check if highscore was reached
			if (mHighScoreSave)
			{
				int32 highscoreEntries = mHighScoreSave->GetHighScores().Num();
				// either less than 10 entries or the newScore is within the topten
				mReachedHighScore = highscoreEntries < 10 || mHighScoreSave->GetHighScores()[highscoreEntries - 10].mScore < mScore;
			}
		}

	}*/
}
/*
void UDUserWidget_PostGameMenu::OnChangeName(FText name)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("name changed.")));
	int32 maxCharacters = 30;
	FString strName = name.ToString();
	if (strName.Len() > maxCharacters)
	{
		strName.RemoveAt(maxCharacters, strName.Len() - 1 - maxCharacters);
		name.FromString(strName);
	}

	mPlayerName = strName;
}
*/