// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TrainVehicle.generated.h"

class ARailTrack;
class UMaterialInstanceDynamic;
class UMaterial;

UCLASS()
class TINYTRAIN_API ATrainVehicle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainVehicle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// railtrack to extend
	UPROPERTY(EditInstanceOnly)
	ARailTrack* m_RailTrackToExtend;

	int m_numberWagons;
	float m_speed;

	virtual void Reset() override;
	void Play();
	void Pause();
	void Stop();

private:
	UStaticMesh* m_staticWagonMesh;
	UStaticMeshComponent* m_firstVehicleMesh;
	TArray<UStaticMeshComponent*> m_wagonMeshes;
	float m_dist;
	float m_wagonLength;
	float m_gapBetweenWagons;
	bool m_bPlaying;

	UMaterial* m_defaultMaterial;
	UMaterialInstanceDynamic* m_currentMaterialInstance;
};
