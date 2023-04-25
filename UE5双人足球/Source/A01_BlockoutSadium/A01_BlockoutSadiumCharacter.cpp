// Copyright Epic Games, Inc. All Rights Reserved.



#include "A01_BlockoutSadiumCharacter.h"

#include "GravityField.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PowerUp.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"

//////////////////////////////////////////////////////////////////////////
// AA01_BlockoutSadiumCharacter

AA01_BlockoutSadiumCharacter::AA01_BlockoutSadiumCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	PrimaryActorTick.bCanEverTick=true;
	PrimaryActorTick.bStartWithTickEnabled =true;
	PrimaryActorTick.bAllowTickOnDedicatedServer=true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Footsteps Component"));
	AudioComponent->SetupAttachment(RootComponent);

	AudioComponent->SetAutoActivate(false);
	bIsFootstepsPlaying = false;

	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempMesh -> SetupAttachment(GetMesh(),TEXT("hand_r"));
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable ->SetupAttachment(GetMesh(),TEXT("Hand_r"));

}

//////////////////////////////////////////////////////////////////////////
// Input

void AA01_BlockoutSadiumCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AA01_BlockoutSadiumCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AA01_BlockoutSadiumCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AA01_BlockoutSadiumCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AA01_BlockoutSadiumCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AA01_BlockoutSadiumCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AA01_BlockoutSadiumCharacter::TouchStopped);

	PlayerInputComponent->BindAction("ForcePush",IE_Pressed,this,&AA01_BlockoutSadiumCharacter::ForcePush);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AA01_BlockoutSadiumCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AA01_BlockoutSadiumCharacter::SprintRelease);

	PlayerInputComponent->BindAction("Ghost",IE_Pressed,this,&AA01_BlockoutSadiumCharacter::Ghost);
	
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AA01_BlockoutSadiumCharacter::UseItem);

	PlayerInputComponent->BindAction("Hook",IE_Pressed,this,&AA01_BlockoutSadiumCharacter::Hook);
	
	PlayerInputComponent->BindAction("LaunchField",IE_Pressed,this,&AA01_BlockoutSadiumCharacter::LaunchField);

	PlayerInputComponent->BindAction("SpawnBomb", IE_Pressed, this, &AA01_BlockoutSadiumCharacter::SpawnBomb);

	PlayerInputComponent->BindAction("SpawnBlast", IE_Pressed, this, &AA01_BlockoutSadiumCharacter::SpawnBlast);
}

void AA01_BlockoutSadiumCharacter::UseItem()
{
	ServerUseItem();
}

void AA01_BlockoutSadiumCharacter::MulticastGhostField_Implementation()
{
	if(NS_GhostField)
	{
		UNiagaraComponent* GhostFieldComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_GhostField,
			FVector(this->GetActorLocation().X,this->GetActorLocation().Y,0), this->GetActorRotation());
		if(WhetherFieldLifeTime)
		{
			GhostFieldComponent->SetNiagaraVariableFloat(FString("LifeTime"),LongGhostFieldLifeTime);
			GhostFieldComponent->SetNiagaraVariableFloat(FString("wpo"),LongWpo);
		}
		else
		{
			GhostFieldComponent->SetNiagaraVariableFloat(FString("LifeTime"),ShortGhostFieldLifeTime);
			GhostFieldComponent->SetNiagaraVariableFloat(FString("wpo"),ShortWpo);
		}
	}
}

void AA01_BlockoutSadiumCharacter::HookCoolDown()
{
	HookStillCool = false;
	GetWorld()->GetTimerManager().ClearTimer(HookTimerHandle);
}

void AA01_BlockoutSadiumCharacter::LaunchField()
{
	ServerLaunchField();
}

void AA01_BlockoutSadiumCharacter::MulticastLaunchField_Implementation()
{
	if(NS_Field)
	{
		UNiagaraComponent* FieldComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Field,
			FieldLocation, this->GetActorRotation());
		if(WhetherFieldLifeTime)
		{
			FieldComponent->SetNiagaraVariableFloat(FString("Lifetime"),LongFieldLifeTime);
		}
		else
		{
			FieldComponent->SetNiagaraVariableFloat(FString("Lifetime"),ShortFieldLifeTime);
		}
	}
}

void AA01_BlockoutSadiumCharacter::FieldCoolDown()
{
	FieldStillCool = false;
	WhetherSpawnBullet = false;
	GetWorld()->GetTimerManager().ClearTimer(FieldTimerHandle);
}

void AA01_BlockoutSadiumCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, FieldLocation);
	
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, WhetherFieldLifeTime);
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, ShortFieldLifeTime);
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, LongFieldLifeTime);
	
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, WhetherGhostFieldLifeTime);
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, ShortGhostFieldLifeTime);
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, LongGhostFieldLifeTime);
	
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, ShortWpo);
	DOREPLIFETIME(AA01_BlockoutSadiumCharacter, LongWpo);
}

void AA01_BlockoutSadiumCharacter::SpawnBomb()
{
	if(BombClass)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 20;
		AActor* SpawnedBomb = GetWorld()->SpawnActor(BombClass, &SpawnLocation);
		if(SpawnedBomb)
		{
			Cast<ABomb>(SpawnedBomb)->ProjectileMovementComponent->Velocity = GetFollowCamera()->GetForwardVector() * Cast<ABomb>(SpawnedBomb)->InitialSpeed;
		}
	}
}

void AA01_BlockoutSadiumCharacter::SpawnBlast()
{
	if(NS_Blaster)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 30;
		UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Blaster, SpawnLocation);
		if(SpawnedEffect)
		{
			FVector ParticleVelocity = GetFollowCamera()->GetForwardVector() * 2500;
			SpawnedEffect->SetVariableVec3(FName("User.SpawnVelocity"), ParticleVelocity);
			SpawnedEffect->SetVariableObject(FName("User.ClassToTell"), this);
		}
		
	}
}

void AA01_BlockoutSadiumCharacter::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);
	UE_LOG(LogTemp, Warning, TEXT("Particle Hit Something!"));
	for(int i = 0; i < Data.Num(); i++)
	{
		if(BlasterExplosionClass)
		{
			ABlasterExplosion* SpawnedExplosion = Cast<ABlasterExplosion>(GetWorld()->SpawnActor(BlasterExplosionClass,&Data[i].Position));
			if(SpawnedExplosion)
			{
				SpawnedExplosion->VelocityAmount = 1000;
				SpawnedExplosion->StrainAmount = 500000;
				SpawnedExplosion->SetActorLocation(Data[i].Position);
				SpawnedExplosion->Explode();
			}
		}
	}
}

void AA01_BlockoutSadiumCharacter::StartFootsteps()
{
	if(AudioComponent->Sound && !bIsFootstepsPlaying)
	{
		AudioComponent->Play();
		bIsFootstepsPlaying = true;
	}
}

void AA01_BlockoutSadiumCharacter::ResetFootsteps()
{
	if(AudioComponent->Sound)
	{
		AudioComponent->SetTriggerParameter(FName("On Stop"));
		bIsFootstepsPlaying = false;
	}
}

void AA01_BlockoutSadiumCharacter::ServerLaunchField_Implementation()
{

	if(!FieldStillCool)
	{
		if(!WhetherSpawnBullet)
		{
			FVector Offset = this->GetActorRotation().RotateVector(FVector(50, 0, 60));
			FVector StartPosition = Offset + GetActorLocation();
	
			AGravityFieldBullet* SpawnedBullet = (AGravityFieldBullet*) GetWorld()->SpawnActor(BulletClass, &StartPosition);
			SpawnedBullet->Parent = this;

			SpawnedBullet->SetActorRotation(GetActorRotation());
			WhetherSpawnBullet = true;
		}
		else
		{
			FieldStillCool = true;
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);
			for (AActor* Actor:Actors)
			{
				if(Cast<AGravityFieldBullet>(Actor)&&Cast<AGravityFieldBullet>(Actor)->Parent==this)
				{
					FieldLocation = FVector(Actor->GetActorLocation().X,Actor->GetActorLocation().Y,Actor->GetActorLocation().Z);
					AGravityField* SpawnedField = (AGravityField*) GetWorld()->SpawnActor(FieldClass, &FieldLocation);
					SpawnedField->Parent = this;
					if(Cast<AGravityFieldBullet>(Actor)->CurrentLifetime<=2.5)
					{
						WhetherFieldLifeTime = false;
						SpawnedField->CurrentLifetime=ShortFieldLifeTime;
					}
					else
					{
						WhetherFieldLifeTime =true;
					}
					MulticastLaunchField();
					Cast<AGravityFieldBullet>(Actor)->CurrentLifetime = Cast<AGravityFieldBullet>(Actor)->MaximumLifetime;
				}
			}
			GetWorld()->GetTimerManager().SetTimer(FieldTimerHandle, this, &AA01_BlockoutSadiumCharacter::FieldCoolDown, 20, true);
		}
	}
	
}

void AA01_BlockoutSadiumCharacter::ServerHook_Implementation()
{
	if(!HookStillCool)
	{
		Cable ->SetVisibility(true);
		const FVector Start = FollowCamera->GetComponentLocation();
		const FVector ForwardVector = FollowCamera->GetForwardVector();
	
	
		const FVector End = Start + ForwardVector * 1256;

		FHitResult HitData(ForceInit);
	
		TArray<AActor*> ActorsToIgnore;
	
		ActorsToIgnore.Add(this);
	
		if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false)) {
			if (HitData.GetActor()) {
				UE_LOG(LogClass, Warning, TEXT
					("We Hit this Actor:. %s"), *HitData.GetActor()->GetName());
				ProcessTraceHit(HitData);
			} else
			{
				// The trace did not return an Actor
				// An error has occurred
				// Record a message in the error log
			}
		} else 
		{
			// We did not hit an Actor
		}
		HookStillCool = true;
		GetWorld()->GetTimerManager().SetTimer(HookTimerHandle, this, &AA01_BlockoutSadiumCharacter::HookCoolDown, 5, true);
	}
}

void AA01_BlockoutSadiumCharacter::ServerGhost_Implementation()
{
	if(!GhostStillCool)
	{
		if(!WhetherLeaveGhost)
		{
			WhetherLeaveGhost = true;
			RecordLocation = this->GetActorLocation();
			CurrentGhostTime = 0;
			MulticastGhost();
		}
		else
		{
			if(CurrentGhostTime<=MaximumGhostTime/2)
			{
				WhetherGhostFieldLifeTime = false;
			}
			else
			{
				WhetherGhostFieldLifeTime = true;
			}
			HookStillCool = false;
			GhostStillCool = true;
			GetWorld()->GetTimerManager().SetTimer(GhostTimerHandle, this, &AA01_BlockoutSadiumCharacter::GhostCoolDown, 5, true);
			SetActorLocation(RecordLocation);
			MulticastGhostField();
		}
	}
}

void AA01_BlockoutSadiumCharacter::GhostCoolDown()
{
	WhetherLeaveGhost = false;
	GhostStillCool = false;
	GetWorld()->GetTimerManager().ClearTimer(GhostTimerHandle);
}

void AA01_BlockoutSadiumCharacter::MulticastGhost_Implementation()
{
	if(NS_Ghost)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(NS_Ghost,this->GetMesh(),this->GetFName(),
			this->GetActorLocation(),this->GetActorRotation(),
			EAttachLocation::KeepRelativeOffset,false);
	}
}

void AA01_BlockoutSadiumCharacter::MulticastForcePush_Implementation()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * ForceDistanceOffset;
	if(NS_ForcePush)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_ForcePush, End, this->GetActorRotation());
	}
	
}

void AA01_BlockoutSadiumCharacter::ServerUseItem_Implementation()
{
	if(CurrentPowerUp)
    {
		UE_LOG(LogTemp, Warning, TEXT("A player used an item!"));
		CurrentPowerUp->Use(this);
		CurrentPowerUp = nullptr;
    }

}

void AA01_BlockoutSadiumCharacter::ServerForcePush_Implementation()
{
	MulticastForcePush();
	TArray<FHitResult>HitResults;
	const FVector Start = this->GetActorLocation();
	const FVector End = Start + this->GetActorForwardVector()*ForceDistanceOffset;

	const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(ForceDistanceOffset));
	const bool Sweep = GetWorld()->SweepMultiByChannel(HitResults,End,End,this->GetActorQuat(),
		ECC_WorldDynamic,CubeShape);

	

	for(auto HitResult:HitResults)
	{
		if(HitResult.GetActor() != this)
		{
			UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(HitResult.GetActor()->GetRootComponent());
			if(!HitMesh)
			{
				return;
			}
			HitMesh->AddRadialImpulse(Start,ForceDistanceOffset*2,ForcePower,
				ERadialImpulseFalloff::RIF_Linear,true);
		}
	}
}

void AA01_BlockoutSadiumCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AA01_BlockoutSadiumCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AA01_BlockoutSadiumCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AA01_BlockoutSadiumCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AA01_BlockoutSadiumCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		StartFootsteps();
	}
	else if(GetMovementComponent()->Velocity.IsZero() || GetMovementComponent()->IsFalling())
    {
		ResetFootsteps();
    }

}

void AA01_BlockoutSadiumCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		StartFootsteps();
	}
	else if(GetMovementComponent()->Velocity.IsZero() || GetMovementComponent()->IsFalling())
    {
     ResetFootsteps();
    }

}

void AA01_BlockoutSadiumCharacter::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed=750.f;
	if(AudioComponent->Sound)
	{
		AudioComponent->SetFloatParameter("RepeatSpeed", RunLoopSpeed);
	}
}

void AA01_BlockoutSadiumCharacter::SprintRelease()
{
	GetCharacterMovement()->MaxWalkSpeed=500.f;
	if(AudioComponent->Sound)
	{
		AudioComponent->SetFloatParameter("RepeatSpeed", WalkLoopSpeed);
	}
}

void AA01_BlockoutSadiumCharacter::ForcePush()
{
	ServerForcePush();
}

void AA01_BlockoutSadiumCharacter::Ghost()
{
	ServerGhost();
}

void AA01_BlockoutSadiumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(WhetherLeaveGhost)
	{
		CurrentGhostTime += DeltaTime;
		if(CurrentGhostTime >= MaximumGhostTime)
		{
			WhetherLeaveGhost = false;
			GhostStillCool = true;
			GetWorld()->GetTimerManager().SetTimer(GhostTimerHandle, this, &AA01_BlockoutSadiumCharacter::GhostCoolDown, 5, true);
		}
	}
	if(WhetherHooked == true)
	{
		FVector VectorLength = HookLocation-GetActorLocation();
		LaunchCharacter(VectorLength*DeltaTime*500,true,true);
		WhetherHooked = false;
		Cable->SetVisibility(false);
		TempMesh -> SetWorldLocation(GetActorLocation());
	}

	if(BeCatch == true)
	{
		FVector VectorLength = CatchLocation-GetActorLocation();
		LaunchCharacter(VectorLength*0.5,true,false);
	}
}

bool AA01_BlockoutSadiumCharacter::Trace(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End, FHitResult& HitOut, ECollisionChannel CollisionChannel, bool ReturnPhysMat)
{
	if (!World)
	{
		return false;
	}
	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);
	
	TraceParams.bTraceComplex = true;

	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	TraceParams.AddIgnoredActors(ActorsToIgnore);
	
	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);
	// Perform our trace
	World->LineTraceSingleByChannel (
		HitOut, //result
		Start, //start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
		);
	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

void AA01_BlockoutSadiumCharacter::Hook()
{
	ServerHook();
}

void AA01_BlockoutSadiumCharacter::ProcessTraceHit(FHitResult& HitOut)
{
	if(HitOut.GetActor()&&HitOut.Distance<=1250)
	{
		HookLocation = HitOut.Location;
		TempMesh ->SetWorldLocation(HookLocation);
		WhetherHooked = true;
	}
}


