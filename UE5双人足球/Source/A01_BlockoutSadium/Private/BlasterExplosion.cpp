// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterExplosion.h"

ABlasterExplosion::ABlasterExplosion()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	FieldSystemComponent->SetupAttachment(RootComponent);
	ExplosionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Hitbox"));
	ExplosionRadius->SetupAttachment(RootComponent);
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	CullingField = CreateDefaultSubobject<UCullingField>(TEXT("Culling Field"));
}

void ABlasterExplosion::Explode()
{
	RadialVector->SetRadialVector(VelocityAmount, GetActorLocation());
	RadialFalloff->SetRadialFalloff(StrainAmount,0,1,0,ExplosionRadius->GetScaledSphereRadius(),GetActorLocation(),EFieldFalloffType::Field_FallOff_None);
	CullingField->SetCullingField(RadialFalloff, RadialVector, EFieldCullingOperationType::Field_Culling_Outside);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, CullingField);
	Destroy();
}
