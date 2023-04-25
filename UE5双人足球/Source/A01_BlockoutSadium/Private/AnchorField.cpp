// Fill out your copyright notice in the Description page of Project Settings.


#include "AnchorField.h"

AAnchorField::AAnchorField()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(FieldSystemComponent);
	UniformInteger = CreateDefaultSubobject<UUniformInteger>(TEXT("Uniform Integer"));
	CullingField = CreateDefaultSubobject<UCullingField>(TEXT("Culling Field"));
	BoxFalloff = CreateDefaultSubobject<UBoxFalloff>(TEXT("Box Falloff"));
}

void AAnchorField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BoxFalloff->SetBoxFalloff(1.0f, 0.0f, 1.0f, 0.0f, BoxCollision->GetComponentTransform(), EFieldFalloffType::Field_FallOff_None);
	UniformInteger->SetUniformInteger(3);
	CullingField->SetCullingField(BoxFalloff,UniformInteger,EFieldCullingOperationType::Field_Culling_Outside);
	FieldSystemComponent->AddFieldCommand(true, EFieldPhysicsType::Field_DynamicState, nullptr, CullingField);
}
