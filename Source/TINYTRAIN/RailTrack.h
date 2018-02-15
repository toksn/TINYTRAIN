// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "RailTrack.generated.h"

class USplineComponent;
class USplineMeshComponent;
class UTriggerBoxComponent;

UCLASS()
class TINYTRAIN_API ARailTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARailTrack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (Category = "myObject", OverrideNativeName = "mySpline"))
	USplineComponent* m_spline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Spline Static Mesh", Category = "myObject", OverrideNativeName = "mySplineStaticMesh"))
	UStaticMesh* m_splineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "use distance method", Category = "myObject", OverrideNativeName = "use distance method"))
	bool m_bUseDistanceMethod;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "uniform points", Category = "myObject", OverrideNativeName = "This button does distribute splinepoints uniformly along the spline.\nThe length of each segment is equal to the staticMesh length."))
	bool m_bButtonPressed;

	//UPROPERTY(EditAnywhere)
	//UTriggerBoxComponent* m_loosingHitBox;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Reset() override;

	bool RemeshSpline();
	bool AddDrawnLinePoints(TArray<FVector> pointsToAdd);

	float GetSplineMeshLength();

private:
	void UniformSplinePoints();
	bool RemeshSplineByPoints(/*int32 indexStart = 0, int32 indexEnd = -1*/);	
	bool RemeshSplineByDistance();

	TArray<USplineMeshComponent*> m_SegmentMeshes;	
	TArray<FSplinePoint> m_initialSplinePoints;
};
