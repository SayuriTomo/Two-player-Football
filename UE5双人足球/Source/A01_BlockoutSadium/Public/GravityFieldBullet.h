// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "SportGameState.h"
#include "GravityFieldBullet.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API AGravityFieldBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityFieldBullet();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* BulletMesh;
	UPROPERTY(Replicated,EditAnywhere)AActor* Parent;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ASportGameState* GameState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;
	UMaterialInstanceDynamic* MaterialInstance;

	UFUNCTION(Server, Unreliable)void ServerChangeColour(bool TeamOne);
	UFUNCTION(NetMulticast, Unreliable)void MulticastChangeColour(bool TeamOne);
	
	UPROPERTY(EditAnywhere)float MovementSpeed ;
	UPROPERTY(EditAnywhere)float MaximumLifetime;
	UPROPERTY(EditAnywhere)float CurrentLifetime;
	
};

