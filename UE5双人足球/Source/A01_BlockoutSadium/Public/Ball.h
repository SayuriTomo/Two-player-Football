// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A01_BlockoutSadium/A01_BlockoutSadiumCharacter.h"
#include "Net/UnrealNetwork.h"
#include "SportGameState.h"

#include "Ball.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ASportGameState* GameState;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)UStaticMeshComponent* BallMesh;
	UPROPERTY(Replicated)AA01_BlockoutSadiumCharacter* LastPlayer;
	UFUNCTION()void OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;
	UMaterialInstanceDynamic* MaterialInstance;

	UFUNCTION(Server, Unreliable)void ServerChangeColour(bool TeamOne);
	UFUNCTION(NetMulticast, Unreliable)void MulticastChangeColour(bool TeamOne);
};
