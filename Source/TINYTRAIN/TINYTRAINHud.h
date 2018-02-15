// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TINYTRAINHud.generated.h"

/**
 * 
 */
USTRUCT()
struct FTimed2DLine
{
	GENERATED_USTRUCT_BODY()

	float fTimeToLive;
	float fThickness;
	FLinearColor color;
	FVector2D ptStart;
	FVector2D ptEnd;
};

UCLASS()
class TINYTRAIN_API ATINYTRAINHud : public AHUD
{
	GENERATED_BODY()
public:
	ATINYTRAINHud();

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	void DrawLineToHud(FVector2D ptStart, FVector2D ptEnd, FLinearColor LineColor, float LineThickness, float LifeTime);

	virtual void DrawLineToHud(
		float StartScreenX, float StartScreenY,
		float EndScreenX, float EndScreenY,
		FLinearColor LineColor,
		float LineThickness = 0.0F,
		float LifeTime = -1.0F);

	virtual void DrawRectToHud(
		FBox2D rect,
		FLinearColor LineColor,
		float LineThickness = 0.0F,
		float LifeTime = -1.0F);

	virtual void ClearLifetimeLines(bool clearPersistentLines = false);

	bool m_bShowLifetimeLines;
private:
	TArray<FTimed2DLine> m_linesToDraw;
};
