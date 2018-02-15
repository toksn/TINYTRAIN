// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "DUserWidget_HUD.h"



//UDUserWidget_HUD::UDUserWidget_HUD()
//{
//	
//}

void UDUserWidget_HUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	bool rc = false;
	float fDegree = 0.0f;

	if (m_arrowImage)
	{
		FIntPoint resolution = GEngine->GameUserSettings->GetScreenResolution();
		//FIntPoint resolution = GEngine->GameUserSettings->GetResolutionScaleInformation();

		m_screenMid = GEngine->GameViewport->Viewport->GetSizeXY();
		resolution.X /= m_screenMid.X;
		resolution.Y /= m_screenMid.Y;
		m_screenMid /= 2.0;

		uint32 m_intRadius = (uint32)(m_screenMid.GetMin() * 0.9);

		APlayerController* PC = Cast<APlayerController>(this->GetOwningPlayerPawn()->GetController());
		if (PC)
		{
			// todo: get actual target location
			FVector targetLoc_world(690, -340, 40);

			FVector2D targetLoc_screen;
			rc = PC->ProjectWorldLocationToScreen(targetLoc_world, targetLoc_screen);
			
			static const auto CVarScreenPercentage = IConsoleManager::Get().FindTConsoleVariableDataFloat(TEXT("r.ScreenPercentage"));
			float screenPercentage = CVarScreenPercentage->GetValueOnGameThread() / 100.0f;
			targetLoc_screen /= screenPercentage;

			// go from midScreen to target screen location with defined distance

			FVector2D arrowDir = targetLoc_screen - m_screenMid;
			float curDist = arrowDir.Size();
			arrowDir.Normalize();
			FVector2D pointForArrow = m_screenMid;
			if(curDist<m_intRadius)
				pointForArrow += arrowDir * curDist;
			else
				pointForArrow += arrowDir * m_intRadius;

			FVector2D coordYDir(0, -1);
			// angle between two 2d lines = acos(dotproduct(line1,line2)); in radiant --> convert to degree
			fDegree = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(arrowDir, coordYDir)));

			// angle is [0,180] at the moment
			// if cross product is > 0 there is an negative angle
			if (FVector2D::CrossProduct(arrowDir, coordYDir) > 0.0f)
				fDegree = -fDegree;
			
			UCanvasPanelSlot* canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(m_arrowImage);
			float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
			canvasSlot->SetPosition(pointForArrow /= Scale);
			
			m_arrowImage->SetRenderAngle(fDegree);
		}
	}
}

void UDUserWidget_HUD::ButtonPause()
{
}
