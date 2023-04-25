// Fill out your copyright notice in the Description page of Project Settings.


#include "WreckingBall.h"

// Sets default values
AWreckingBall::AWreckingBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint"));
	Rope = CreateDefaultSubobject<UCableComponent>(TEXT("Rope"));
	Top->SetupAttachment(RootComponent);
	Ball->SetupAttachment(RootComponent);
	PhysicsConstraint->SetupAttachment(RootComponent);
	Rope->SetupAttachment(RootComponent);
	Ball->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AWreckingBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWreckingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

