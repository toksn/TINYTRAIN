// Fill out your copyright notice in the Description page of Project Settings.

#include "TINYTRAIN.h"
#include "TINYTRAINHud.h"


ATINYTRAINHud::ATINYTRAINHud()
{
	m_bShowLifetimeLines = true;
}

void ATINYTRAINHud::BeginPlay()
{
}

void ATINYTRAINHud::DrawHUD()
{
	FTimed2DLine* curLine;
	for( int i = 0; i < m_linesToDraw.Num(); i++)
	{
		curLine = &m_linesToDraw[i];
		
		// draw the line
		if(m_bShowLifetimeLines)
			DrawLine(curLine->ptStart.X, curLine->ptStart.Y, curLine->ptEnd.X, curLine->ptEnd.Y, curLine->color, curLine->fThickness);

		// advance time for the line
		if (curLine->fTimeToLive >= 0.0f)
		{
			curLine->fTimeToLive -= RenderDelta;

			// line life time over - kill it (with fire)
			if (curLine->fTimeToLive <= 0.0f)
			{
				m_linesToDraw.RemoveAt(i, 1, false);
				i--;
			}
		}
	}	
}

void ATINYTRAINHud::DrawLineToHud(FVector2D ptStart, FVector2D ptEnd, FLinearColor LineColor, float LineThickness, float LifeTime)
{
	DrawLineToHud(ptStart.X, ptStart.Y, ptEnd.X, ptEnd.Y, LineColor, LineThickness, LifeTime);
}

void ATINYTRAINHud::DrawLineToHud(float StartScreenX, float StartScreenY, float EndScreenX, float EndScreenY, FLinearColor LineColor, float LineThickness, float LifeTime)
{
	FTimed2DLine line;
	line.ptStart.X = StartScreenX;
	line.ptStart.Y = StartScreenY;

	line.ptEnd.X = EndScreenX;
	line.ptEnd.Y = EndScreenY;

	line.color = LineColor;
	line.fThickness = LineThickness;
	line.fTimeToLive = LifeTime;

	m_linesToDraw.Add(line);
}

void ATINYTRAINHud::DrawRectToHud(FBox2D rect, FLinearColor LineColor, float LineThickness, float LifeTime)
{
	DrawLineToHud(rect.Min.X, rect.Min.Y, rect.Max.X, rect.Min.Y, LineColor, LineThickness, LifeTime);
	DrawLineToHud(rect.Min.X, rect.Min.Y, rect.Min.X, rect.Max.Y, LineColor, LineThickness, LifeTime);
	DrawLineToHud(rect.Max.X, rect.Max.Y, rect.Min.X, rect.Max.Y, LineColor, LineThickness, LifeTime);
	DrawLineToHud(rect.Max.X, rect.Max.Y, rect.Max.X, rect.Min.Y, LineColor, LineThickness, LifeTime);
}

void ATINYTRAINHud::ClearLifetimeLines(bool clearPersistentLines)
{
	if (clearPersistentLines)
	{
		for (int i = 0; i < m_linesToDraw.Num(); i++)
		{
			if (m_linesToDraw[i].fTimeToLive >= 0.0f)
			{ 
				m_linesToDraw.RemoveAt(i);
				i--;
			}			
		}
	}
	else
		m_linesToDraw.Empty();
}
