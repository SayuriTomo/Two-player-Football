// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FloorMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	FloorMesh->SetupAttachment(RootComponent);
	
	bReplicates = true;
	FloorMesh ->SetIsReplicated(true);
	FloorCollider=CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	FloorCollider->SetupAttachment(FloorMesh);
	FloorCollider ->SetIsReplicated(true);
	NetPriority=3.0f;
	
	FloorCollider->OnComponentBeginOverlap.AddDynamic(this,&AFloor::OnBeginOverlap);
	
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		FloorMesh->SetMaterial(0,MaterialInstance);
	}
	GameState= Cast<ASportGameState>(GetWorld()->GetGameState());
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFloor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
						ServerChangeColour(true,LastPlayer);
					}
					else
					{
						ServerChangeColour(false,LastPlayer);
					}
				}
				
			}
		}
	}
}

void AFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFloor, LastPlayer);
}

void AFloor::ServerChangeColour_Implementation(bool TeamOne, AA01_BlockoutSadiumCharacter* Player)
{
	if(!BeOwned)
	{
		MulticastChangeColour(TeamOne);
		if(TeamOne)
		{
			WhichTeamOwn = 1;
		}
		else
		{
			WhichTeamOwn = 2;
		}
		BeOwned = true;
	}
	else
	{
		if(TeamOne)
		{
			if(WhichTeamOwn == 1)
			{
				Player->GetCharacterMovement()->MaxWalkSpeed = 750.f;
			}
		}
		else
		{
			if(WhichTeamOwn == 2)
			{
				Player->GetCharacterMovement()->MaxWalkSpeed = 750.f;
			}
		}
	}
	
}

void AFloor::MulticastChangeColour_Implementation(bool TeamOne)
{
	if(MaterialInstance)
	{
		if(TeamOne)
		{
			MaterialInstance->SetVectorParameterValue("Colour", GameState->TeamOneColour);
		}
		else
		{
			MaterialInstance->SetVectorParameterValue("Colour", GameState->TeamTwoColour);
		}
	}
}


