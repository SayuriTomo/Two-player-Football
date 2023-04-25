// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "Components/AudioComponent.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	GoalMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	GoalMesh->SetupAttachment(RootComponent);

	GoalCollider=CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	GoalCollider->SetupAttachment(GoalMesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Footsteps Component"));
	AudioComponent->SetupAttachment(RootComponent);

	AudioComponent->SetAutoActivate(false);
	bIsFootstepsPlaying = false;
	
	bReplicates=true;

}

void AGoal::ServerSpawnGoalParticles_Implementation(FVector SpawnLocation, bool TeamOne)
{
	MulticastSpawnGoalParticles(SpawnLocation, TeamOne);
}

void AGoal::MulticastSpawnGoalParticles_Implementation(FVector SpawnLocation, bool TeamOne)
{
	if(NS_GoalExplosion)
	{
		UNiagaraComponent* GoalExplosion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_GoalExplosion,
	   SpawnLocation, GetActorForwardVector().Rotation());
		if(TeamOne)
		{
			GoalExplosion->SetNiagaraVariableLinearColor(FString("User.Colour"), GameState->TeamOneColour);
		}
		else
		{
			GoalExplosion->SetNiagaraVariableLinearColor(FString("User.Colour"), GameState->TeamTwoColour);
		}
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	GoalCollider->OnComponentBeginOverlap.AddDynamic(this,&AGoal::OnBeginOverlap);

	GameState = Cast<ASportGameState>(GetWorld()->GetGameState());
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(WhetherSoundStart)
	{
		CurrentTime += DeltaTime;
		if (CurrentTime >= 3)
		{
			WhetherSoundStart = false;
			ResetSound();
			CurrentTime = 0;
		}
	}

}

void AGoal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(OtherActor&&OtherActor!=this)
		{
			UE_LOG(LogTemp,Warning,TEXT("Goal overlapped with something"));
			if(Cast<ABall>(OtherActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("GOOOOOOOOOOAL!"))
				ABall* Ball = Cast<ABall>(OtherActor);
				if(Ball->LastPlayer)
				{
					if(GameState)
					{
						if(GameState->TeamOne.Contains(Ball->LastPlayer->GetPlayerState()))
						{
							GameState->TeamOneGoalScored(1);
							StartSound();
							ServerSpawnGoalParticles(Ball->GetActorLocation(), true);
							if(GameState->TeamOneScore>=GameState->TeamTwoScore)
							{
								if(AudioComponent->Sound)
								{
									AudioComponent->SetFloatParameter("RepeatSpeed", HighLoopSpeed);
								}
								else
								{
									AudioComponent->SetFloatParameter("RepeatSpeed", LowLoopSpeed);
								}
							}
						}
						else
						{
							GameState->TeamTwoGoalScored(1);
							StartSound();
							ServerSpawnGoalParticles(Ball->GetActorLocation(), false);
							if(GameState->TeamTwoScore>=GameState->TeamOneScore)
							{
								if(AudioComponent->Sound)
								{
									AudioComponent->SetFloatParameter("RepeatSpeed", HighLoopSpeed);
								}
								else
								{
									AudioComponent->SetFloatParameter("RepeatSpeed", LowLoopSpeed);
								}
							}
						}
					}
				}
				Ball->Destroy();
				WhetherSoundStart = true;
				GetWorld()->SpawnActor(BallClass, &BallSpawnLocation);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("This is not a ball!"));
			}
		}
	}
	
}

void AGoal::StartSound()
{
	if(HasAuthority())
	{
		if(AudioComponent->Sound && !bIsFootstepsPlaying)
		{
			AudioComponent->Play();
			bIsFootstepsPlaying = true;
		}
	}
}

void AGoal::ResetSound()
{
	if(HasAuthority())
	{
		if(AudioComponent->Sound)
		{
			AudioComponent->SetTriggerParameter(FName("On Stop"));
			bIsFootstepsPlaying = false;
		}
	}
}


