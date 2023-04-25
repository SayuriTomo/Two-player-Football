// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NiagaraSystem.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GravityField.h"
#include "GravityFieldBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Bomb.h"
#include "NiagaraDataInterfaceExport.h"
#include "BlasterExplosion.h"
#include "CableComponent.h"
#include "A01_BlockoutSadiumCharacter.generated.h"
class APowerUp;


UCLASS(config=Game)
class AA01_BlockoutSadiumCharacter : public ACharacter, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AA01_BlockoutSadiumCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere)
	float ForceDistanceOffset = 100;
	UPROPERTY(EditAnywhere)
	float ForcePower = 7500;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_ForcePush;

	UPROPERTY(EditAnywhere)
	float MaximumGhostTime = 5;
	UPROPERTY(EditAnywhere)
	float CurrentGhostTime;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_Ghost;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_GhostField;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_Field;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void ForcePush();
	void SprintStart();
	void SprintRelease();

	void Ghost();
	bool WhetherLeaveGhost = false;
	bool GhostStillCool = false;
	FVector RecordLocation;
	FTimerHandle GhostTimerHandle;
	void GhostCoolDown();

	virtual void Tick(float DeltaTime) override;
	
	bool Trace(UWorld* World,TArray<AActor*>& ActorsToIgnore,const FVector& Start,
		const FVector& End,FHitResult& HitOut,ECollisionChannel CollisionChannel,bool ReturnPhysMat);
	void Hook();
	void ProcessTraceHit(FHitResult& HitOut);
	
	bool WhetherHooked = false;
	FVector HookLocation;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Reliable)void ServerForcePush();

	void UseItem();
	UFUNCTION(Server, Reliable)void ServerUseItem();
	APowerUp* CurrentPowerUp;

	UFUNCTION(NetMulticast, Unreliable)void MulticastForcePush();

	UFUNCTION(Server, Reliable)void ServerGhost();
	UFUNCTION(NetMulticast, Unreliable)void MulticastGhost();
	UFUNCTION(NetMulticast, Unreliable)void MulticastGhostField();
	UPROPERTY(Replicated,EditAnywhere)bool WhetherGhostFieldLifeTime;
	UPROPERTY(Replicated,EditAnywhere)float LongGhostFieldLifeTime = 10;
	UPROPERTY(Replicated,EditAnywhere)float ShortGhostFieldLifeTime = 5;
	UPROPERTY(Replicated,EditAnywhere)float ShortWpo = 150;
	UPROPERTY(Replicated,EditAnywhere)float LongWpo = 300;
	
	
	UFUNCTION(Server, Reliable)void ServerHook();
	FTimerHandle HookTimerHandle;
	void HookCoolDown();
	bool HookStillCool = false;
	
	UPROPERTY(EditAnywhere,Category= "Bullet Spawning")TSubclassOf<AGravityFieldBullet> BulletClass;
	void LaunchField();
	UPROPERTY(Replicated,EditAnywhere)FVector FieldLocation;
	UFUNCTION(Server, Reliable)void ServerLaunchField();
	UFUNCTION(NetMulticast, Unreliable)void MulticastLaunchField();
	bool WhetherSpawnBullet = false;
	UPROPERTY(Replicated,EditAnywhere)bool WhetherFieldLifeTime;
	UPROPERTY(Replicated,EditAnywhere)float LongFieldLifeTime = 15;
	UPROPERTY(Replicated,EditAnywhere)float ShortFieldLifeTime = 8;

	UPROPERTY(EditAnywhere,Category= "Field Spawning")TSubclassOf<AGravityField> FieldClass;
	bool BeCatch = false;
	FVector CatchLocation;
	FTimerHandle FieldTimerHandle;
	void FieldCoolDown();
	bool FieldStillCool = false;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere)TSubclassOf<ABomb> BombClass;
	void SpawnBomb();
	
	UPROPERTY(EditAnywhere)UNiagaraSystem* NS_Blaster;
    void SpawnBlast();
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
		UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

	UPROPERTY(EditAnywhere)TSubclassOf<ABlasterExplosion> BlasterExplosionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
     UAudioComponent* AudioComponent;

	bool bIsFootstepsPlaying;

	void StartFootsteps();
	void ResetFootsteps();

	const float WalkLoopSpeed = 0.42f;
    const float RunLoopSpeed = 0.21f;

	UPROPERTY(EditAnywhere)UStaticMeshComponent* TempMesh;
	UPROPERTY(EditAnywhere)UCableComponent* Cable;
	
};