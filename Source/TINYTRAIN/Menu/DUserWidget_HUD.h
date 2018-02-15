// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DUserWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TINYTRAIN_API UDUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UDUserWidget_HUD();
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	//bool GetDirectionToGoals();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDVariables")
	FLinearColor mColArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDVariables")
	UImage* m_arrowImage;

	UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	void ButtonPause();

	//UFUNCTION(BlueprintCallable, Category = "HUDWidgetFunctions")
	//void ButtonUpToggle();
private:
	FVector2D m_screenMid;
};
