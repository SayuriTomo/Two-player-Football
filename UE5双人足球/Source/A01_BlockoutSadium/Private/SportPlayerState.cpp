// Fill out your copyright notice in the Description page of Project Settings.


#include "SportPlayerState.h"

void ASportPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	if(HasAuthority())
	{
		GetWorld()->GetGameState()->AddPlayerState(this);
    }

}
