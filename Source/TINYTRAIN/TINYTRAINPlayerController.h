// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "TINYTRAINPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class ATINYTRAINPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATINYTRAINPlayerController();

	//virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
	//virtual void BeginPlay() override;
};


