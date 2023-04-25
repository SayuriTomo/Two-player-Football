// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SportGameState.h"
#include "Components/BoxComponent.h"
#include "GravityField.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API AGravityField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityField();
	UPROPERTY(EditAnywhere)UBoxComponent* FieldCollider;
	UPROPERTY(Replicated,EditAnywhere)AActor* Parent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ASportGameState* GameState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION() void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere)float MaximumLifetime;
	UPROPERTY(EditAnywhere)float CurrentLifetime;
	

};
