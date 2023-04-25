// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "Components/SphereComponent.h"
#include "BlasterExplosion.generated.h"

/**
 * 
 */
UCLASS()
class A01_BLOCKOUTSADIUM_API ABlasterExplosion : public AFieldSystemActor
{
	GENERATED_BODY()
public:
	ABlasterExplosion();
	UPROPERTY(EditAnywhere)USphereComponent* ExplosionRadius;
	UPROPERTY(EditAnywhere)UCullingField* CullingField;
	UPROPERTY(EditAnywhere)URadialVector* RadialVector;
	UPROPERTY(EditAnywhere)URadialFalloff* RadialFalloff;
	float VelocityAmount;
	float StrainAmount;
	void Explode();
};
