// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "Components/BoxComponent.h"
#include "AnchorField.generated.h"

/**
 * 
 */
UCLASS()
class A01_BLOCKOUTSADIUM_API AAnchorField : public AFieldSystemActor
{
	GENERATED_BODY()
public:
	AAnchorField();
	UPROPERTY(EditAnywhere)UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere)UBoxFalloff* BoxFalloff;
	UPROPERTY(EditAnywhere)UUniformInteger* UniformInteger;
	UPROPERTY(EditAnywhere)UCullingField* CullingField;

	virtual void OnConstruction(const FTransform& Transform) override;
};
