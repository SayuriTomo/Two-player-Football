// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Ball.h"
#include "SportGameState.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Goal.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* GoalMesh;
	UPROPERTY(EditAnywhere)UBoxComponent* GoalCollider;
	
	UPROPERTY(EditAnywhere,Category= "Ball Spawning")FVector BallSpawnLocation;
	UPROPERTY(EditAnywhere,Category= "Ball Spawning")TSubclassOf<ABall> BallClass;

	ASportGameState* GameState;

	UPROPERTY(EditAnywhere)UNiagaraSystem* NS_GoalExplosion;
	UFUNCTION(Server, Unreliable)void ServerSpawnGoalParticles(FVector SpawnLocation, bool TeamOne);
	UFUNCTION(NetMulticast, Unreliable)void MulticastSpawnGoalParticles(FVector SpawnLocation, bool TeamOne);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

	bool bIsFootstepsPlaying;

	void StartSound();
	void ResetSound();
	bool WhetherSoundStart = false;
	float CurrentTime = 0;

	const float LowLoopSpeed = 0.4f;
	const float HighLoopSpeed = 0.2f;
	
};
