// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TrainVehicle.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "RailTrack.h"


// Sets default values
ATrainVehicle::ATrainVehicle()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MeshVisualAsset;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> MyMaterial;
		//ConstructorHelpers::FObjectFinderOptional<UParticleSystem> MyParticleEffect;
		FConstructorStatics()
			//: MeshVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"))
			//: MeshVisualAsset(TEXT("/Game/MyContent/Meshes/diamond.diamond"))
			: MeshVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"))
			, MyMaterial(TEXT("/Game/UsedAssets/Materials/M_ColoredMaterial.M_ColoredMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	
	PrimaryActorTick.bCanEverTick = true;
	m_dist = 0.0f;
	m_staticWagonMesh = ConstructorStatics.MeshVisualAsset.Get();
	m_firstVehicleMesh = NULL;	
	m_RailTrackToExtend = NULL;
	//m_color = STONE_COLOR::COUNT;

	// number of wagons
	m_numberWagons = 3;
	// length and gap of wagons
	m_wagonLength = 0.8f;
	m_gapBetweenWagons = 10.0f;
	// speed
	m_speed = 300.0f;
	m_bPlaying = true;

	// root will be a simple scene obj
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	
	// first vehicle mesh
	m_firstVehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstVehicleMesh"));
	m_firstVehicleMesh->SetStaticMesh(m_staticWagonMesh);
	m_firstVehicleMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_firstVehicleMesh->SetWorldScale3D(FVector(m_wagonLength, m_wagonLength / 2.0f, m_wagonLength / 2.0f));
	m_firstVehicleMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// use material
	m_defaultMaterial = ConstructorStatics.MyMaterial.Get();
	m_firstVehicleMesh->SetMaterial(0, m_defaultMaterial);
	m_currentMaterialInstance = m_firstVehicleMesh->CreateAndSetMaterialInstanceDynamic(0);
	m_currentMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Blue.Desaturate(0.1));

	// hp?
}

// Called when the game starts or when spawned
void ATrainVehicle::BeginPlay()
{
	Super::BeginPlay();

	// create wagon meshes
	for (int i = 0; i < m_numberWagons; i++)
	{
		UStaticMeshComponent *wagonMesh = NewObject<UStaticMeshComponent>(this);
		wagonMesh->SetStaticMesh(m_staticWagonMesh);
		wagonMesh->SetMobility(EComponentMobility::Movable);
		wagonMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		wagonMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		wagonMesh->RegisterComponent();
		wagonMesh->SetWorldScale3D(FVector(m_wagonLength, m_wagonLength/2.0f, m_wagonLength / 2.5f));
		wagonMesh->SetMaterial(0, m_defaultMaterial);
		wagonMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Blue.Desaturate(0.3));
		
		m_wagonMeshes.Add(wagonMesh);
	}
}

// Called every frame
void ATrainVehicle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// position the firstVehicle + wagonMeshes along the spline
	if (m_bPlaying && m_RailTrackToExtend)
	{
		m_dist += DeltaTime * m_speed;
		if (m_dist > m_RailTrackToExtend->m_spline->GetSplineLength())
			m_dist = m_RailTrackToExtend->m_spline->GetSplineLength();

		FVector loc;
		FRotator rot;	
		ESplineCoordinateSpace::Type space = ESplineCoordinateSpace::Local;
		loc = m_RailTrackToExtend->m_spline->GetLocationAtDistanceAlongSpline(m_dist, space);
		rot = m_RailTrackToExtend->m_spline->GetRotationAtDistanceAlongSpline(m_dist, space);
		m_firstVehicleMesh->SetRelativeLocation(loc);
		m_firstVehicleMesh->SetRelativeRotation(rot);

		float wagonDist = m_dist;
		for (UStaticMeshComponent* wagon : m_wagonMeshes)
		{
			//wagonDist -= m_wagonLength * 1.1f;
			wagonDist -= m_wagonLength * 100.0f + m_gapBetweenWagons;
			if (wagonDist < 0.0f)
				wagonDist = 0.0f;

			loc = m_RailTrackToExtend->m_spline->GetLocationAtDistanceAlongSpline(wagonDist, space);
			rot = m_RailTrackToExtend->m_spline->GetRotationAtDistanceAlongSpline(wagonDist, space);
			wagon->SetRelativeLocation(loc);
			wagon->SetRelativeRotation(rot);
		}
	}
}

void ATrainVehicle::Reset()
{
	Super::Reset();

	m_dist = 0.0f;
}
void ATrainVehicle::Play()
{
	m_bPlaying = true;
}
void ATrainVehicle::Pause()
{
	m_bPlaying = false;
}
void ATrainVehicle::Stop()
{
	m_bPlaying = false;
	Reset();
}