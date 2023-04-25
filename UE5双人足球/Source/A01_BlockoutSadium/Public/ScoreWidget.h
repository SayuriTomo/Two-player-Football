// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SportGameState.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class A01_BLOCKOUTSADIUM_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))UTextBlock* TeamOneScore;
	UPROPERTY(meta=(BindWidget))UTextBlock* TeamTwoScore;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	ASportGameState* GameState;
	
};
