// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BallMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	BallMesh->SetupAttachment(RootComponent);

	BallMesh->SetSimulatePhysics(true);

	bReplicates = true;
	BallMesh ->SetIsReplicated(true);

	NetPriority=3.0f;

	BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnHitBegin);
    BallMesh->SetCollisionObjectType(ECC_PhysicsBody);


}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		BallMesh->SetMaterial(0,MaterialInstance);
    }
	GameState= Cast<ASportGameState>(GetWorld()->GetGameState());
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && OtherActor != this)
	{
		if(Cast<AA01_BlockoutSadiumCharacter>(OtherActor))
		{
			if(HasAuthority())
			{
				LastPlayer = Cast<AA01_BlockoutSadiumCharacter>(OtherActor);
				if(GameState)
				{
					if(GameState->TeamOne.Contains(LastPlayer->GetPlayerState()))
					{
						ServerChangeColour(true);
					}
					else
					{
						ServerChangeColour(false);
					}
				}
			}
		}
	}
}

void ABall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABall, LastPlayer);
}

void ABall::ServerChangeColour_Implementation(bool TeamOne)
{
	MulticastChangeColour(TeamOne);
}

void ABall::MulticastChangeColour_Implementation(bool TeamOne)
{
	if(MaterialInstance)
	{
		if(TeamOne)
		{
			MaterialInstance->SetVectorParameterValue("Colour", GameState->TeamOneColour);
		}
		else
		{
			MaterialInstance->SetVectorParameterValue
			("Colour", GameState->TeamTwoColour);
		}
	}
}

