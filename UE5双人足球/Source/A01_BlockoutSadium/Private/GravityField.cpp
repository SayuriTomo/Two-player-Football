// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityField.h"

#include "A01_BlockoutSadium/A01_BlockoutSadiumCharacter.h"

// Sets default values
AGravityField::AGravityField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FieldCollider=CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	FieldCollider->SetupAttachment(RootComponent);
	bReplicates = true;
	FieldCollider->SetIsReplicated(true);
	NetPriority=3.0f;
	MaximumLifetime = 10;
}

// Called when the game starts or when spawned
void AGravityField::BeginPlay()
{
	Super::BeginPlay();
	GameState= Cast<ASportGameState>(GetWorld()->GetGameState());
	FieldCollider->OnComponentBeginOverlap.AddDynamic(this,&AGravityField::OnBeginOverlap);
	FieldCollider->OnComponentEndOverlap.AddDynamic(this, &AGravityField::OnEndOverlap);
}

// Called every frame
void AGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentLifetime += DeltaTime;
	if(CurrentLifetime >= MaximumLifetime)
	{
		Destroy();
	}
}

void AGravityField::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(OtherActor && OtherActor != this)
		{
			if(!Parent)return;
			if(Parent == OtherActor)return;

			if(Cast<AA01_BlockoutSadiumCharacter>(OtherActor))
			{
				Cast<AA01_BlockoutSadiumCharacter>(OtherActor)->BeCatch = true;
				Cast<AA01_BlockoutSadiumCharacter>(OtherActor)->CatchLocation = GetActorLocation();
			}
			UE_LOG(LogTemp, Warning, TEXT("Field Working"));
		}
	}
}
void AGravityField::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(HasAuthority())
	{
		if(OtherActor && OtherActor != this)
		{
			if(!Parent)return;
			if(Parent == OtherActor)return;

			if(Cast<AA01_BlockoutSadiumCharacter>(OtherActor))
			{
				Cast<AA01_BlockoutSadiumCharacter>(OtherActor)->BeCatch = false;
			}
			UE_LOG(LogTemp, Warning, TEXT("Field Not Working"));
		}
	}
}

void AGravityField::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGravityField, Parent);
}


