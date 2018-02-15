// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TINYTRAIN.h"
#include "TINYTRAINPawn.h"
#include "RailTrack.h"
#include "TrainVehicle.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/HUD.h"
#include "TINYTRAINHud.h"
#include "TINYTRAINGameState.h"

ATINYTRAINPawn::ATINYTRAINPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	m_RailTrackToExtend = NULL;
	m_trainVehicle = NULL;
	m_bDrawingArea_printed = false;
	m_fMinDistance = 10.0;

	// Create a camera for the player
	m_camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("mainCamera"));
	m_camera->AttachTo(RootComponent);
	
	m_camera->SetWorldLocation(FVector(0.0, 800.0, 0.0));
	m_camera->SetWorldRotation(FRotator(0.0, -90.0, 0.0));

	m_drawingArea.Min = FVector2D(300, 100);
	m_drawingArea.Max = FVector2D(600, 400);

	m_bRotateCameraWithTrack = true;
	m_fCamFlowTime = 0.5;
	m_fCamCurrentTime = 0.0;
}

// Called when the game starts or when spawned
void ATINYTRAINPawn::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameViewport()->GetEngineShowFlags()->SetSplines(true);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
		PC->SetViewTarget(this);

	// Spawn a train at the spline location
	FVector train_location = m_RailTrackToExtend->GetActorLocation();	
	FRotator train_rotation = m_RailTrackToExtend->GetActorRotation();
	// move train a little up
	train_location += train_rotation.Quaternion().GetUpVector() * 25.0;
	
	m_trainVehicle = GetWorld()->SpawnActor<ATrainVehicle>(train_location, train_rotation);
	m_trainVehicle->SetOwner(this);
	m_trainVehicle->m_RailTrackToExtend = m_RailTrackToExtend;
	

	m_camLocationOld = m_camLocationNew = m_RailTrackToExtend->m_spline->GetLocationAtTime(1.0f, ESplineCoordinateSpace::World);
	m_camRotationOld = m_camRotationNew = m_RailTrackToExtend->m_spline->GetRotationAtTime(1.0f, ESplineCoordinateSpace::World);
}

void ATINYTRAINPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		ATINYTRAINHud* hud = Cast<ATINYTRAINHud>(PC->GetHUD());
		
		if (!m_bDrawingArea_printed)
		{
			hud->DrawRectToHud(m_drawingArea, FColor::Yellow, 1.0);
			m_bDrawingArea_printed = true;
		}
		ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
		if (gs && gs->GetGameState() == ATINYTRAINGameState::TINYTRAIN_GAMESTATE::DRAWING && hud)
		{
			// get current mouse location
			FVector temp, curPosition;
			FVector2D curScreenPos;
			if (PC->GetMousePosition(curScreenPos.X, curScreenPos.Y))
			{
				
				if(m_drawingArea.IsInside(curScreenPos))
				if (m_linePoints.Num() && FVector2D::Distance(curScreenPos, m_linePoints.operator[](m_linePoints.Num() - 1).screenPos) > m_fMinDistance)
				{
					if (PC->DeprojectMousePositionToWorld(curPosition, temp))
					{						
						hud->DrawLineToHud(m_linePoints.operator[](m_linePoints.Num() - 1).screenPos, curScreenPos, FColor::Red, 1.0, 2.5);
						FMouseLinePoint curLinePoint;
						curLinePoint.screenPos = curScreenPos;
						curLinePoint.worldPos = curPosition;

						m_linePoints.Add(curLinePoint);
					}
				}
			}
		}
	}
}

void ATINYTRAINPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &ATINYTRAINPawn::TriggerClick);
	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Released, this, &ATINYTRAINPawn::TriggerClick_Released);
	PlayerInputComponent->BindAction("Decline", EInputEvent::IE_Pressed, this, &ATINYTRAINPawn::OnPressed_Decline);
	PlayerInputComponent->BindAction("Accept", EInputEvent::IE_Pressed, this, &ATINYTRAINPawn::OnPressed_Accept);
}

void ATINYTRAINPawn::OnPressed_Decline()
{
	// different stuff to happen depending on the gamestate
	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs)
	{
		gs->CallCurrentMenuDecline();
	}
}

void ATINYTRAINPawn::OnPressed_Accept()
{
	// different stuff to happen depending on the gamestate
	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs)
	{
		gs->CallCurrentMenuAccept();
	}
}

void ATINYTRAINPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	float fZ = 1200;
	m_camLocation = m_camLocationNew;
	m_camRotation = m_camRotationNew;
	
	if (m_fCamCurrentTime < m_fCamFlowTime)
	{
		m_fCamCurrentTime += DeltaTime;
		float fAlpha = m_fCamCurrentTime / m_fCamFlowTime;
		if (fAlpha >= 1.0f)
		{
			fAlpha = 1.0f;
			m_fCamCurrentTime = m_fCamFlowTime;
			m_camLocationOld = m_camLocationNew;
			m_camRotationOld = m_camRotationNew;
		}

		// interpolate location
		m_camLocation = FMath::InterpEaseOut(m_camLocationOld, m_camLocationNew, fAlpha, 2.5f);
		// interpolate rotation
		m_camRotation = FMath::InterpEaseOut(m_camRotationOld, m_camRotationNew, fAlpha, 2.5f);
	}

	// set rotation
	m_camRotation.Pitch = -90.0f;
	OutResult.Rotation = m_camRotation;
	// set location
	m_camLocation.Z = fZ;
	OutResult.Location = m_camLocation;
		
	/*
	// preserve location Z value
	float fZ = 1200;

	if (m_RailTrackToExtend&& m_RailTrackToExtend->m_spline)
	{
		

		// set camera location to last spline point
		OutResult.Location = m_RailTrackToExtend->m_spline->GetLocationAtTime(1.0, ESplineCoordinateSpace::World);
		OutResult.Location.Z = fZ;
		
		// set camera rotation to last spline point rotation
		if(m_bRotateCameraWithTrack)
			OutResult.Rotation = m_RailTrackToExtend->m_spline->GetRotationAtTime(1.0, ESplineCoordinateSpace::World);
		OutResult.Rotation.Pitch = -90.0f;
	}
	else if(m_RailTrackToExtend)
	{
		OutResult.Location = m_RailTrackToExtend->GetActorLocation();
		OutResult.Location.Z = fZ;
		OutResult.Rotation = FRotator(0.0f, -90.0f, 0.0f);
	}*/
		
}

void ATINYTRAINPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATINYTRAINPawn::TriggerClick_Released()
{
	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs && gs->GetGameState() == ATINYTRAINGameState::TINYTRAIN_GAMESTATE::DRAWING)
	{
		// add points to railtrack
		AddDrawnLineToRailTrack();

		gs->SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::IDLE);

		MoveCameraToLastRail();
	}
}

void ATINYTRAINPawn::MoveCameraToLastRail()
{
	// add new point for camera to move to
	// set camera location to last spline point
	m_camLocationOld = m_camLocation;
	m_camLocationNew = m_RailTrackToExtend->m_spline->GetLocationAtTime(1.0, ESplineCoordinateSpace::World);
	m_fCamCurrentTime = 0.0f;

	// set camera rotation to last spline point rotation
	if (m_bRotateCameraWithTrack)
	{
		m_camRotationOld = m_camRotation;
		m_camRotationNew = m_RailTrackToExtend->m_spline->GetRotationAtTime(1.0, ESplineCoordinateSpace::World);

		FRotator tempRot = m_camRotationOld - m_camRotationNew;
		if (tempRot.Yaw > 180.0f)
			m_camRotationOld.Yaw -= 360.0f;
		else if (tempRot.Yaw < -180.0f)
			m_camRotationOld.Yaw += 360.0f;
	}
}

// should actually be called projectDrawnLineToSplineSpace()
void ATINYTRAINPawn::AddDrawnLineToRailTrack()
{
	ATINYTRAINHud* hud = NULL;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
		hud = Cast<ATINYTRAINHud>(PC->GetHUD());

	if (hud && m_RailTrackToExtend && m_linePoints.Num() > 1 )
	{
		TArray<FVector> splinePointsToAdd;
		FVector lastSplinePoint, splineDir, curSquarePt;
		//FMouseLinePoint curDrawLinePt;

		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;
		splinePointsToAdd.Reserve(m_linePoints.Num() - 1);

		// get spline point location and direction at the very end
		splineDir = m_RailTrackToExtend->m_spline->GetDirectionAtTime(1.0, space);
		splineDir.Normalize();
		lastSplinePoint = m_RailTrackToExtend->m_spline->GetLocationAtTime(1.0, space);

		

		// *********** 1:
		// normalize on-screen square in screenspace
		// (2D bounding box -> longest side to be 1.0)
		FVector2D min = m_drawingArea.Max;
		FVector2D max = m_drawingArea.Min;
		FVector2D DrawnLine_BoundingBox_Dimension, newSquare;
		float squareLengthPx = 1.0;
		for (FMouseLinePoint curDrawLinePt : m_linePoints)
		{
			min.X = FMath::Min(curDrawLinePt.screenPos.X, min.X);
			min.Y = FMath::Min(curDrawLinePt.screenPos.Y, min.Y);

			max.X = FMath::Max(curDrawLinePt.screenPos.X, max.X);
			max.Y = FMath::Max(curDrawLinePt.screenPos.Y, max.Y);
		}
		DrawnLine_BoundingBox_Dimension.X = max.X -min.X;
		DrawnLine_BoundingBox_Dimension.Y = max.Y - min.Y;
		squareLengthPx = FMath::Max(DrawnLine_BoundingBox_Dimension.X, DrawnLine_BoundingBox_Dimension.Y);

		// normalize the screenpositions
		for (int i = 0; i < m_linePoints.Num(); i++)
		{
			m_linePoints[i].screenPos -= min;
			m_linePoints[i].screenPos /= squareLengthPx;
		}

		
		// *********** 2:
		// repos on-screen square to have origin in first linepoint
		FVector2D initPos = m_linePoints[0].screenPos;
		for (int i = 0; i < m_linePoints.Num(); i++)
		{
			m_linePoints[i].screenPos -= initPos;
						
			// rotate by 90 degree to fit world orientation (camera is rotated by 90!?) - this has to be in relation to camera
			float temp = m_linePoints[i].screenPos.X;
			m_linePoints[i].screenPos.X = -m_linePoints[i].screenPos.Y;
			m_linePoints[i].screenPos.Y = temp;			
			//m_linePoints[i].screenPos = m_linePoints[i].screenPos.GetRotated(-90.0);
			//m_linePoints[i].screenPos.Y *= -1.0;
		}

		// *********** 3:
		// multiply with direction and position the points in relation to the lastSplinePoint
		for (int i = 1; i < m_linePoints.Num(); i++)
		{
			curSquarePt.Z = 0;
			curSquarePt.X = m_linePoints[i].screenPos.X;
			curSquarePt.Y = m_linePoints[i].screenPos.Y;

			// apply world space scale
			float splineSegmentLength = m_RailTrackToExtend->GetSplineMeshLength();
			curSquarePt *= splineSegmentLength;

			// make it go in the splineDir
			FRotator rot = splineDir.Rotation();
			curSquarePt = rot.RotateVector(curSquarePt);
			//FQuat qat = FQuat::FindBetweenNormals(FVector::UpVector, splineDir);
			//curSquarePt = qat.RotateVector(curSquarePt);
			//curSquarePt *= splineDir;

			// position it in relation to the last square point
			curSquarePt += lastSplinePoint;

			splinePointsToAdd.Add(curSquarePt);
		}

		m_RailTrackToExtend->AddDrawnLinePoints(splinePointsToAdd);
	}
}

void ATINYTRAINPawn::TriggerClick()
{
	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs && gs->GetGameState() == ATINYTRAINGameState::TINYTRAIN_GAMESTATE::IDLE)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			gs->SetGameState(ATINYTRAINGameState::TINYTRAIN_GAMESTATE::DRAWING);

			m_linePoints.Empty();

			// get current mouse location
			FVector worldPos, worldDir;
			FVector2D screenPos;
			if (PC->DeprojectMousePositionToWorld(worldPos, worldDir) && PC->GetMousePosition(screenPos.X, screenPos.Y))
			{
				FMouseLinePoint curLinePoint;
				curLinePoint.screenPos = screenPos;
				curLinePoint.worldPos = worldPos;

				m_linePoints.Add(curLinePoint);
			}

		}
	}
}