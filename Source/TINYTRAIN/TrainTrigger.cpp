// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINGameState.h"
#include "TrainVehicle.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
//#include "Runtime/Engine/Classes/Materials/Material.h"
#include "TrainTrigger.h"

ATrainTrigger::ATrainTrigger()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional <UStaticMesh> MeshVisualAsset;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> defMat;
		FConstructorStatics()
			: MeshVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"))
			, defMat(TEXT("/Game/UsedAssets/Materials/M_ColoredMaterial.M_ColoredMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	m_obstacleMeshComp = NULL;
	m_obstacleMesh = NULL;
	m_bWinTrigger = false;

	m_obstacleMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_obstacleMeshComp;
	
	m_obstacleMesh = ConstructorStatics.MeshVisualAsset.Get();
	
	// Handle material
	m_obstacleMeshComp->SetMobility(EComponentMobility::Movable);
	m_obstacleMeshComp->SetMaterial(0, ConstructorStatics.defMat.Get());
	m_currentMaterial = m_defaultMaterial = m_obstacleMeshComp->CreateAndSetMaterialInstanceDynamic(0);
	UpdateMaterialColor();
	
	m_obstacleMeshComp->SetStaticMesh(m_obstacleMesh);

	// COLLISION
	m_obstacleMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_obstacleMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_obstacleMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	// collision channel?
	//m_obstacleMeshComp->SetCollisionObjectType(COLLISIONCHANNEL_TRAIN_OBSTACLE);

	//m_obstacleMeshComp->RegisterComponent();

	m_obstacleMeshComp->bGenerateOverlapEvents = true;
	m_obstacleMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ATrainTrigger::OnTriggerEnter);
	m_obstacleMeshComp->OnComponentEndOverlap.AddDynamic(this, &ATrainTrigger::OnTriggerExit);
}

#if WITH_EDITOR
void ATrainTrigger::OnConstruction(const FTransform& Transform)
{
	UpdateMaterialColor();
}

void ATrainTrigger::PostEditMove(bool bFinished)
{
	UpdateMaterialColor();
}
#endif

void ATrainTrigger::UpdateMaterialColor()
{
	// only update the color when current material is still the default material
	if (m_currentMaterial && m_currentMaterial == m_defaultMaterial)
	{
		if (m_bWinTrigger)
			m_currentMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Green);
		else
			m_currentMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Red);
	}
}

void ATrainTrigger::OnTriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (m_bWinTrigger)
		LevelWon(Cast<ATrainVehicle>(OtherActor));
	else
		LevelFailed(Cast<ATrainVehicle>(OtherActor));
}

void ATrainTrigger::LevelWon(ATrainVehicle* train)
{
#ifdef _DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("GOAL TRIGGER ENTERED. ENDING LEVEL NOW!")));
#endif

	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs && gs->GetLevelState() == TINYTRAIN_LEVELSTATE::RUNNING)
	{
		// end level
		gs->EndLevel(TINYTRAIN_LEVELSTATE::WON);

		// pause train
		if (train)
		{
			train->Pause();
			// todo: effect of people coming out of the train or something?
		}
	}
}


void ATrainTrigger::LevelFailed(ATrainVehicle* train)
{
#ifdef _DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("LOOSING TRIGGER ENTERED. ENDING LEVEL NOW!")));
#endif

	ATINYTRAINGameState* gs = (ATINYTRAINGameState*)GetWorld()->GetGameState();
	if (gs && gs->GetLevelState() == TINYTRAIN_LEVELSTATE::RUNNING)
	{
		// endlevel
		gs->EndLevel(TINYTRAIN_LEVELSTATE::FAILED);

		// pause train
		if (train)
		{
			train->Pause();

			// todo: derail train --> physics?
		}
	}
}

void ATrainTrigger::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ATrainTrigger::SetMeshIsVisible(bool bTriggerOnly)
{
	// check for mesh comp
	// todo: create when null
	if (m_obstacleMeshComp)
		m_obstacleMeshComp->SetVisibility(bTriggerOnly);
}

bool ATrainTrigger::IsMeshVisible()
{
	if (m_obstacleMeshComp)
		return m_obstacleMeshComp->IsVisible();
	else
		return false;
}




