// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A01_BlockoutSadium/A01_BlockoutSadiumCharacter.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "SportGameState.h"

#include "Floor.generated.h"

UCLASS()
class A01_BLOCKOUTSADIUM_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ASportGameState* GameState;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)UStaticMeshComponent* FloorMesh;
	UPROPERTY(EditAnywhere)UBoxComponent* FloorCollider;
	UPROPERTY(Replicated)AA01_BlockoutSadiumCharacter* LastPlayer;
	
	UFUNCTION() void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;
	UMaterialInstanceDynamic* MaterialInstance;

	UFUNCTION(Server, Unreliable)void ServerChangeColour(bool TeamOne, AA01_BlockoutSadiumCharacter* Player);
	UFUNCTION(NetMulticast, Unreliable)void MulticastChangeColour(bool TeamOne);

	UPROPERTY(Replicated)bool BeOwned;
	UPROPERTY(Replicated)float WhichTeamOwn;
};
