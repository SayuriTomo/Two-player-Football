// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "Ball.h"
#include "MultiBallPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class A01_BLOCKOUTSADIUM_API AMultiBallPowerUp : public APowerUp
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)FVector BallSpawnLocation;
    UPROPERTY(EditAnywhere)TSubclassOf<ABall> BallClass;

	virtual void Use(AA01_BlockoutSadiumCharacter* OwningPlayer) override;
	
};
