// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "SportPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class A01_BLOCKOUTSADIUM_API ASportPlayerState : public APlayerState
{
	GENERATED_BODY()
	virtual void ClientInitialize(AController* C) override;

};
