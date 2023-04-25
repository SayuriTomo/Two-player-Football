// Fill out your copyright notice in the Description page of Project Settings.


#include "SportGameState.h"

ASportGameState::ASportGameState()
{
	TeamOneScore=0;
	TeamTwoScore=0;
	TeamOneColour = FLinearColor::Blue;
    TeamTwoColour = FLinearColor::Red;

	
}

void ASportGameState::TeamOneGoalScored(int Amount)
{
	if(HasAuthority())
	{
		TeamOneScore += Amount;
	}
}

void ASportGameState::TeamTwoGoalScored(int Amount)
{
	if(HasAuthority())
	{
		TeamTwoScore+=Amount;
	}
}

void ASportGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASportGameState, TeamOneScore);
    DOREPLIFETIME(ASportGameState, TeamTwoScore);

}

void ASportGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	if(Cast<ASportPlayerState>(PlayerState))
	{
		ASportPlayerState* NewSportPlayerState = Cast<ASportPlayerState>(PlayerState);
		if(TeamOne.Num() < TeamTwo.Num())
		{
			TeamOne.Add(NewSportPlayerState);
		}
		else if(TeamTwo.Num() < TeamOne.Num())
		{
			TeamTwo.Add(NewSportPlayerState);
		}
		else
		{
			if(rand() % 2 == 0)
			{
				TeamOne.Add(NewSportPlayerState);
			}
			else
			{
				TeamTwo.Add(NewSportPlayerState);
			}
		}
	}
}
