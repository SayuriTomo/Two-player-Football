// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "WreckingBall.h"
#include "Lever.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API ALever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALever();
	UPROPERTY(EditAnywhere)
     UStaticMeshComponent* Arm;
    UPROPERTY(EditAnywhere)
     UStaticMeshComponent* Base;
    UPROPERTY(EditAnywhere)
     UPhysicsConstraintComponent* PhysicsConstraint;
	UPROPERTY(EditAnywhere)AWreckingBall* WreckingBall;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
