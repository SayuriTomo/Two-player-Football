// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityFieldBullet.h"

#include "A01_BlockoutSadium/A01_BlockoutSadiumCharacter.h"

// Sets default values
AGravityFieldBullet::AGravityFieldBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BulletMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Field Mesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionProfileName("OverlapAll");
	Parent = nullptr;
	
	bReplicates = true;
	BulletMesh ->SetIsReplicated(true);
	NetPriority=3.0f;

	MovementSpeed = 950;
	MaximumLifetime = 5;
	CurrentLifetime = 0;

}

// Called when the game starts or when spawned
void AGravityFieldBullet::BeginPlay()
{
	Super::BeginPlay();
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		BulletMesh->SetMaterial(0,MaterialInstance);
	}
	GameState= Cast<ASportGameState>(GetWorld()->GetGameState());
	
}

// Called every frame
void AGravityFieldBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLocation = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector Movement = (Forward * MovementSpeed * DeltaTime);
	FVector NewLocation = CurrentLocation + Movement;
	SetActorLocation(NewLocation);
	
	CurrentLifetime += DeltaTime;
	if(CurrentLifetime >= MaximumLifetime)
	{
		Destroy();
	}
	if(HasAuthority())
	{
		if(GameState)
		{
			if(GameState->TeamOne.Contains(Cast<AA01_BlockoutSadiumCharacter>(Parent)->GetPlayerState()))
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

void AGravityFieldBullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGravityFieldBullet, Parent);
}

void AGravityFieldBullet::ServerChangeColour_Implementation(bool TeamOne)
{
	MulticastChangeColour(TeamOne);
}

void AGravityFieldBullet::MulticastChangeColour_Implementation(bool TeamOne)
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

