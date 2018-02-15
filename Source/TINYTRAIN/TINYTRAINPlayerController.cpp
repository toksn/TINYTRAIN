// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TINYTRAIN.h"
#include "TINYTRAINPlayerController.h"

ATINYTRAINPlayerController::ATINYTRAINPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	bAutoManageActiveCameraTarget = false;
}

//void ATINYTRAINPlayerController::BeginPlay()
//{
//	//Super::BeginPlay();
//	PlayerCameraManager->SetViewTarget(GetPawn());
//}
//
//
//void ATINYTRAINPlayerController::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
//{
//	if (GetPawn())
//		this->GetPawn()->CalcCamera(DeltaTime, OutResult);
//	else
//		Super::CalcCamera(DeltaTime, OutResult);
//}