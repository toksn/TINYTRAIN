// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TrainTrigger.generated.h"


class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS()
class TINYTRAIN_API ATrainTrigger : public AActor//AStaticMeshActor
{
	GENERATED_BODY()
public:
	ATrainTrigger();
	
	UFUNCTION()
	virtual void OnTriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (OverrideNativeName = "obstacle mesh component"))
	UStaticMeshComponent* m_obstacleMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Obstacle Mesh", OverrideNativeName = "mesh to use for display and trigger"))
	UStaticMesh* m_obstacleMesh;

	//UProperty(EditAnywhere)
	UMaterialInstanceDynamic* m_currentMaterial;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditMove(bool bFinished) override;
#endif

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Obstacle Mesh visibiltiy", Category = "Obstacle", OverrideNativeName = "whether to only use the mesh as trigger"))
	//bool m_bMeshIsVisible;

	void SetMeshIsVisible(bool bTriggerOnly);
	bool IsMeshVisible();

	UPROPERTY(EditAnywhere)
	bool m_bWinTrigger;

private:
	void UpdateMaterialColor();

	void LevelWon(ATrainVehicle* train);
	void LevelFailed(ATrainVehicle* train);

	UMaterialInstanceDynamic* m_defaultMaterial;
};
