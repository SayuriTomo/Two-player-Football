// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Bomb.generated.h"

/**
 * 
 */
UCLASS()
class A01_BLOCKOUTSADIUM_API ABomb : public AFieldSystemActor
{
	GENERATED_BODY()
public:
	ABomb();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* BombMesh;
	UPROPERTY(EditAnywhere)USphereComponent* ExplosionRadius;
	UPROPERTY(EditAnywhere)UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)URadialFalloff* RadialFalloff;
	UPROPERTY(EditAnywhere)UCullingField* CullingField;
	UPROPERTY(EditAnywhere)URadialVector* RadialVector;
	UPROPERTY(EditAnywhere)float ExplosionStrain = 500000;
	UPROPERTY(EditAnywhere)float ExplosionVelocity = 2000;
	UPROPERTY(EditAnywhere)UNiagaraSystem* NS_Explosion;
	UPROPERTY(EditAnywhere)USoundBase* SB_Explosion;
	float InitialSpeed = 1000;
	float GravityScale = 0.5f;
	UFUNCTION()
     void OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor,
     	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	
};
