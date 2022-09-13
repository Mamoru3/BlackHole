// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UExplosion_Projectile.h"
#include "UFloatingBullet.h"
#include "UGravity_Projectile.h"
#include "Components/SkeletalMeshComponent.h"
#include <chrono>
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "Engine/EngineTypes.h"
#include "UFirstPersonCharacter.generated.h"


class UCameraComponent;


UCLASS(Blueprintable)
class GRAVITY_BALL_API AUFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUFirstPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_changeBulletVar = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_gravCooldown = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_explCooldown = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_zeroGravCooldown = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_mana = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_manaRechargeRate = 0.07;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_zeroGravCost = 70.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_GravHoleCost = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_ExplCost = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_projectileSpawnable = true;		//Can the projectile be spawned

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* FireAnimation;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	UFUNCTION(BlueprintCallable)
		void changeBullCountUp();
	UFUNCTION(BlueprintCallable)
		void changeBullCountDown();
	UFUNCTION(BlueprintCallable)
		void spawnProj();
	UFUNCTION()
		void TimerFunc();
	UFUNCTION()
		void ManaRegen();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

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

	FTimerHandle Cooldown;					//Timer for cooldown
	

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
