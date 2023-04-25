// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "SportPlayerState.h"
#include "SportGameState.generated.h"

/**
 * 
 */
UCLASS()
class A01_BLOCKOUTSADIUM_API ASportGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ASportGameState();
	UPROPERTY(Replicated)int TeamOneScore;
	UPROPERTY(Replicated)int TeamTwoScore;

	void TeamOneGoalScored(int Amount);
	void TeamTwoGoalScored(int Amount);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)TArray<ASportPlayerState*> TeamOne;
	UPROPERTY(EditAnywhere)TArray<ASportPlayerState*> TeamTwo;
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	UPROPERTY(EditAnywhere)FLinearColor TeamOneColour;
    UPROPERTY(EditAnywhere)FLinearColor TeamTwoColour;

};
