// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerBoxComponent.h"
#include "TrainTriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Trigger, meta = (BlueprintSpawnableComponent))
class TINYTRAIN_API UTrainTriggerComponent : public UTriggerBoxComponent
{
public:
	GENERATED_BODY()
	
	UTrainTriggerComponent();

	UFUNCTION()
	virtual void OnTriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		//void OnTriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	virtual void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	bool m_bWinTrigger;
};
