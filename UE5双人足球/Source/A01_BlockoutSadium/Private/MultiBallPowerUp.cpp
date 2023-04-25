// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBallPowerUp.h"

void AMultiBallPowerUp::Use(AA01_BlockoutSadiumCharacter* OwningPlayer)
{
	Super::Use(OwningPlayer);
	if(BallClass)
	{
		GetWorld()->SpawnActor
		(BallClass, &BallSpawnLocation);
	}
}
