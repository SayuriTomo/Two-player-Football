// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ASportGameState>(GetWorld()->GetGameState());
}

void UScoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(GameState)
	{
		TeamOneScore->SetText(FText::AsNumber(GameState->TeamOneScore));
		TeamTwoScore->SetText(FText::AsNumber(GameState->TeamTwoScore));
	}
}
