// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SportGameState.h"
#include "A01_BlockoutSadium/A01_BlockoutSadiumCharacter.h"
#include "PowerUp.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* PowerUpMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void PickUp(AA01_BlockoutSadiumCharacter* OwningPlayer);
	virtual void Use(AA01_BlockoutSadiumCharacter* OwningPlayer);
	UFUNCTION()
	 void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor,
	 	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	 	const FHitResult& SweepResult);

};
