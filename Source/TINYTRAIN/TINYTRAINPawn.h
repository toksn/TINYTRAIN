// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

//#include "TrainVehicle.h";
#include "TINYTRAINPawn.generated.h"

USTRUCT()
struct FMouseLinePoint
{
	GENERATED_USTRUCT_BODY()
	FVector worldPos;
	FVector2D screenPos;

};

class ARailTrack;
class ATrainVehicle;

UCLASS(config=Game)
class ATINYTRAINPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void MoveCameraToLastRail();

	// railtrack to extend
	UPROPERTY(EditInstanceOnly)
	ARailTrack* m_RailTrackToExtend;
	ATrainVehicle* m_trainVehicle;

protected:
	void OnResetVR();
	void TriggerClick();
	void TriggerClick_Released();
	void OnPressed_Decline();
	void OnPressed_Accept();

	void AddDrawnLineToRailTrack();

	bool m_bDrawingArea_printed;
	float m_fMinDistance;
	TArray<FMouseLinePoint> m_linePoints;
	UCameraComponent* m_camera;
	FBox2D m_drawingArea;

	bool m_bRotateCameraWithTrack;

	FVector m_camLocationOld; 
	FVector m_camLocationNew;
	FVector m_camLocation;
	FRotator m_camRotationOld;
	FRotator m_camRotationNew;
	FRotator m_camRotation;
	float m_fCamFlowTime;
	float m_fCamCurrentTime;
};
