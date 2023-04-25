// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CableComponent.h"
#include "WreckingBall.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API AWreckingBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWreckingBall();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* Top;
	UPROPERTY(EditAnywhere)UStaticMeshComponent* Ball;
	UPROPERTY(EditAnywhere)UPhysicsConstraintComponent* PhysicsConstraint;
	UPROPERTY(EditAnywhere)UCableComponent* Rope;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
