// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"

ABomb::ABomb()
{
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb Mesh"));
	BombMesh->SetupAttachment(RootComponent);
	ExplosionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Hitbox"));
	ExplosionRadius->SetupAttachment(FieldSystemComponent);
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	CullingField = CreateDefaultSubobject<UCullingField>(TEXT("Culling Field"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(BombMesh);
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale;
	BombMesh->OnComponentHit.AddDynamic(this, &ABomb::OnHitBegin);
	BombMesh->SetCollisionProfileName("Block All");
	BombMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ABomb::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(NS_Explosion)
    {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NS_Explosion,BombMesh->GetComponentLocation());
    }
    if(SB_Explosion)
    {
    	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Explosion, BombMesh->GetComponentLocation());
    }
	RadialFalloff->SetRadialFalloff(ExplosionStrain, 0, 1, 0,
		ExplosionRadius->GetScaledSphereRadius(),BombMesh->GetComponentLocation(),
		EFieldFalloffType::Field_FallOff_None);
	RadialVector->SetRadialVector(ExplosionVelocity, BombMesh->GetComponentLocation());
	CullingField->SetCullingField(RadialFalloff, RadialVector, EFieldCullingOperationType::Field_Culling_Outside);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, CullingField);
	Destroy();

}
