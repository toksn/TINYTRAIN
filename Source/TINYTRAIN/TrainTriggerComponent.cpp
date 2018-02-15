// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINGameState.h"
#include "TrainTriggerComponent.h"

UTrainTriggerComponent::UTrainTriggerComponent() : Super()
{
	m_bWinTrigger = false;
}

void UTrainTriggerComponent::OnTriggerEnter(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs)
	{
		if (m_bWinTrigger)
		{
#ifdef _DEBUG
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("GOAL TRIGGER ENTERED. ENDING LEVEL NOW!")));
#endif

			gs->EndLevel(TINYTRAIN_LEVELSTATE::WON);
		}
		else
		{
#ifdef _DEBUG
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("LOOSING TRIGGER ENTERED. ENDING LEVEL NOW!")));
#endif

			gs->EndLevel(TINYTRAIN_LEVELSTATE::FAILED);
		}

	}
}

void UTrainTriggerComponent::OnTriggerExit(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}
