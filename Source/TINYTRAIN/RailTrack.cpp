// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "RailTrack.h"
#if WITH_EDITOR
#include "UnrealEd.h"
#endif
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "TriggerBoxComponent.h"

// Sets default values
ARailTrack::ARailTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		//ConstructorHelpers::FObjectFinderOptional<USplineComponent> MeshVisualAsset;
		ConstructorHelpers::FObjectFinderOptional <UStaticMesh> MeshVisualAsset;
		//ConstructorHelpers::FObjectFinderOptional<UMaterial> MyMaterial;
		FConstructorStatics()
			: MeshVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"))
			//, MyMaterial(TEXT("/Game/MyContent/Materials/M_StonesMaterial.M_StonesMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	
	

	m_spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = m_spline;
	m_spline->SetUnselectedSplineSegmentColor(FLinearColor::Red);
	

	//m_loosingHitBox = CreateDefaultSubobject<UTriggerBoxComponent>(TEXT("hitBox"));
	//m_loosingHitBox->SetMobility(EComponentMobility::Movable);
	//m_loosingHitBox->AttachTo(m_spline); //FAttachmentTransformRules::LocationRule::
	
	m_splineMesh = ConstructorStatics.MeshVisualAsset.Get();

	m_bUseDistanceMethod = true;	
}

void ARailTrack::OnConstruction(const FTransform& Transform)
{
	// filter for button pressed
	if (m_bButtonPressed == true)
	{
		//uniform the spline with length of the mesh
		m_bButtonPressed = false;
		UniformSplinePoints();
	}

	RemeshSpline();
}

#if WITH_EDITOR
void ARailTrack::PostEditMove(bool bFinished)
{
	if(bFinished)
	{
		//UniformSplinePoints();
		RemeshSpline();
	}
}
#endif

// Called when the game starts or when spawned
void ARailTrack::BeginPlay()
{
	Super::BeginPlay();

	// save initial spline points to restore on reset
	if (m_initialSplinePoints.Num() == 0 && m_spline)
	{
		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;
		FSplinePoint cur_point;
		for (int i = 0; i < m_spline->GetNumberOfSplinePoints(); i++)
		{
			cur_point.InputKey = i;
			cur_point.ArriveTangent = m_spline->GetArriveTangentAtSplinePoint(i, space);
			cur_point.LeaveTangent = m_spline->GetLeaveTangentAtSplinePoint(i, space);
			cur_point.Position = m_spline->GetLocationAtSplinePoint(i, space);
			cur_point.Rotation = m_spline->GetRotationAtSplinePoint(i, space);
			cur_point.Scale = m_spline->GetScaleAtSplinePoint(i);
			m_initialSplinePoints.Add(cur_point);
		}
	}

	RemeshSpline();
}

void ARailTrack::Reset()
{
	Super::Reset();

	if (m_spline && m_initialSplinePoints.Num())
	{
		// update spline with new points
		m_spline->ClearSplinePoints(false);
		m_spline->AddPoints(m_initialSplinePoints, true);
		RemeshSpline();
	}
		
}

// Called every frame
void ARailTrack::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// update hitbox
	//if (m_spline && m_loosingHitBox)
	//{
	//	FVector loc = m_spline->GetWorldLocationAtTime(1.0);
	//	FRotator rot = m_spline->GetWorldRotationAtTime(1.0);
	//	m_loosingHitBox->SetWorldLocation(loc);
	//	m_loosingHitBox->SetWorldRotation(rot);
	//}
}

bool ARailTrack::RemeshSplineByDistance()
{
	bool rc = false;

	if (m_spline->GetNumberOfSplinePoints() > 1)
	{
		rc = true;

		//this->GetComponentByClass()
		for (USplineMeshComponent* segment : m_SegmentMeshes)
		{
			segment->DestroyComponent();
		}
		m_SegmentMeshes.Empty(m_spline->GetNumberOfSplinePoints() - m_spline->IsClosedLoop() ? 0 : 1);
		
		// this is needed because the spline meshes are not initialized in the array somehow on play
		TArray<USceneComponent*> a;
		m_spline->GetChildrenComponents(false, a);
		for (USceneComponent* segment : a)
		{
			// only destroy it when it is a splinemesh
			if( segment->IsA<USplineMeshComponent>())
				segment->DestroyComponent();
		}
			


		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;
		// try to get "length" of staticMesh used:
		FBox bb = m_splineMesh->GetBoundingBox();
		FVector axisLengths = bb.Max - bb.Min;
		float fSplineMeshLength = axisLengths.X;
		
		for (float currentDistance = 0.0; currentDistance < m_spline->GetSplineLength(); currentDistance += fSplineMeshLength)
		{
			// create UsplineMeshComponent
			USplineMeshComponent *segmentMesh = NewObject<USplineMeshComponent>(this);
			//segmentMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			//segmentMesh->CreationMethod = EComponentCreationMethod::Native;
			segmentMesh->SetStaticMesh(m_splineMesh);
			segmentMesh->SetMobility(EComponentMobility::Movable);
			segmentMesh->AttachToComponent(m_spline, FAttachmentTransformRules::KeepRelativeTransform);
			segmentMesh->RegisterComponent();
			m_SegmentMeshes.Add(segmentMesh);

			FVector start, end, start_tangent, end_tangent;
			// clamp at the end
			float endDist = FMath::Min(currentDistance + fSplineMeshLength, m_spline->GetSplineLength());

			start = m_spline->GetLocationAtDistanceAlongSpline(currentDistance, space);
			start_tangent = m_spline->GetTangentAtDistanceAlongSpline(currentDistance, space);

			end = m_spline->GetLocationAtDistanceAlongSpline(endDist, space);
			end_tangent = m_spline->GetTangentAtDistanceAlongSpline(endDist, space);

			segmentMesh->SetStartAndEnd(start, start_tangent, end, end_tangent);

			segmentMesh->SetStartOffset(FVector2D(0.0, 0.0), true);
			segmentMesh->SetEndOffset(FVector2D(0.0, 0.0), true);			
		}
	}
	return rc;
}

bool ARailTrack::RemeshSpline()
{
	bool rc = false;
	
	if (m_bUseDistanceMethod)
		rc = RemeshSplineByDistance();
	else
		rc = RemeshSplineByPoints();
	return rc;
}


// create a splinemeshcomponent for every section of the spline
bool ARailTrack::RemeshSplineByPoints()
{
	bool rc = false;
	
	if (m_spline->GetNumberOfSplinePoints() > 1)
	{
		rc = true;

		for (USplineMeshComponent* segment : m_SegmentMeshes)
		{
			segment->DestroyComponent();
		}
		m_SegmentMeshes.Empty(m_spline->GetNumberOfSplinePoints() - m_spline->IsClosedLoop()? 0 : 1);


		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;
		// each segment (0,1) (1,2) ... (n-1, n)
		for (int i = 0; i < m_spline->GetNumberOfSplinePoints() - 1; i++)
		{
			// create UsplineMeshComponent
			USplineMeshComponent *segmentMesh = NewObject<USplineMeshComponent>(this);
			segmentMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			segmentMesh->SetStaticMesh(m_splineMesh);
			segmentMesh->SetMobility(EComponentMobility::Movable);
			segmentMesh->AttachToComponent(m_spline, FAttachmentTransformRules::KeepRelativeTransform);
			segmentMesh->RegisterComponent();
			m_SegmentMeshes.Add(segmentMesh);

			// use the controlpoints of the current segment as start and end
			segmentMesh->SetStartAndEnd(m_spline->GetLocationAtSplinePoint(i, space), m_spline->GetTangentAtSplinePoint(i, space),
				m_spline->GetLocationAtSplinePoint(i + 1, space), m_spline->GetTangentAtSplinePoint(i + 1, space));
		}

		// is loop is active -> extra segment from last to first point
		if (m_spline->IsClosedLoop())
		{
			// create UsplineMeshComponent
			USplineMeshComponent *segmentMesh = NewObject<USplineMeshComponent>(this);
			segmentMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			segmentMesh->SetStaticMesh(m_splineMesh);
			segmentMesh->SetMobility(EComponentMobility::Movable);
			segmentMesh->AttachToComponent(m_spline, FAttachmentTransformRules::KeepRelativeTransform);
			segmentMesh->RegisterComponent();
			m_SegmentMeshes.Add(segmentMesh);

			// use the controlpoints of the current segment as start and end
			segmentMesh->SetStartAndEnd(m_spline->GetLocationAtSplinePoint(m_spline->GetNumberOfSplinePoints()-1, space), m_spline->GetTangentAtSplinePoint(m_spline->GetNumberOfSplinePoints()-1, space),
				m_spline->GetLocationAtSplinePoint(0, space), m_spline->GetTangentAtSplinePoint(0, space));
		}
	}
		
	
	

	return rc;
}

void ARailTrack::UniformSplinePoints()
{
	if (m_spline)
	{
		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;

		float currentDistance = 0.0;
		int i = 0;
		TArray<FSplinePoint> points_uniform;
		FSplinePoint cur_point;
		cur_point.InputKey = i++;
		cur_point.ArriveTangent = m_spline->GetTangentAtDistanceAlongSpline(currentDistance, space);
		cur_point.LeaveTangent = cur_point.ArriveTangent;
		cur_point.Position = m_spline->GetLocationAtDistanceAlongSpline(currentDistance, space);
		cur_point.Rotation = m_spline->GetRotationAtDistanceAlongSpline(currentDistance, space);
		cur_point.Scale = m_spline->GetScaleAtDistanceAlongSpline(currentDistance);
		points_uniform.Add(cur_point);

		// try to get "length" of staticMesh used:
		FBox bb = m_splineMesh->GetBoundingBox();
		FVector axisLengths = bb.Max - bb.Min;
		float fSplineMeshLength = axisLengths.X;

		// create a splinepoint each segment
		while(currentDistance < m_spline->GetSplineLength())
		{
			currentDistance += fSplineMeshLength;
			// clamp at the end if needed
			currentDistance = FMath::Min(currentDistance, m_spline->GetSplineLength());

			cur_point.InputKey = i++;
			cur_point.ArriveTangent = m_spline->GetTangentAtDistanceAlongSpline(currentDistance, space);
			cur_point.LeaveTangent = cur_point.ArriveTangent;
			cur_point.Position = m_spline->GetLocationAtDistanceAlongSpline(currentDistance, space);
			cur_point.Rotation = m_spline->GetRotationAtDistanceAlongSpline(currentDistance, space);
			cur_point.Scale = m_spline->GetScaleAtDistanceAlongSpline(currentDistance);
			points_uniform.Add(cur_point);
		};

#if WITH_EDITOR
		// make sure the selected point still exists 
		if( m_spline->GetNumberOfSplinePoints() > points_uniform.Num() )
		{			
			// we have no way of getting the currently selected point index 
			// so always deselect the point when array is getting smaller
			GEditor->SelectNone(false, false);
			GEditor->SelectActor(this, true, true, false, true);
		}
#endif

		// update spline with new points
		m_spline->ClearSplinePoints(false);
		m_spline->AddPoints(points_uniform, true);
	}
}

bool ARailTrack::AddDrawnLinePoints(TArray<FVector> pointsToAdd)
{
	bool rc = false;

	if (pointsToAdd.Num())
	{
		rc = true;
		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;
		for (FVector curPt : pointsToAdd)
			m_spline->AddSplinePoint(curPt, space, false);

		m_spline->UpdateSpline();

		UniformSplinePoints();
		RemeshSpline();
	}

	return rc;
}

float ARailTrack::GetSplineMeshLength()
{
	// try to get "length" of staticMesh used:
	FBox bb = m_splineMesh->GetBoundingBox();
	FVector axisLengths = bb.Max - bb.Min;
	return axisLengths.X;
}