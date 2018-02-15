// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "Engine.h"
#include "TriggerBoxComponent.h"

// Sets default values
UTriggerBoxComponent::UTriggerBoxComponent()
{
	// collision channel?

	bGenerateOverlapEvents = true;
	OnComponentBeginOverlap.AddDynamic(this, &UTriggerBoxComponent::OnTriggerEnter);
	OnComponentEndOverlap.AddDynamic(this, &UTriggerBoxComponent::OnTriggerExit);
}


void UTriggerBoxComponent::OnTriggerEnter(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//#ifdef _DEBUG
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("menustate changed: From %i to %i"), (int32)mPrevMenuState, (int32)mCurrentMenuState));
	//#endif


	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("trigger entered! entering actor: %s"), OtherActor->GetName()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("trigger entered!")));
}

void UTriggerBoxComponent::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("trigger left!")));
}