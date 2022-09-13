// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "UGravity_Projectile.generated.h"

UCLASS(Blueprintable)
class GRAVITY_BALL_API AUGravity_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUGravity_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)			//Function to stop the projectile once cast
		void stopBullet();

	UFUNCTION(BlueprintCallable)			//Function to stop the attract objects once cast
		void castAttractionToObjects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)			//Is the bullet stopped
		bool m_projectileStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)			//Are the actors attracted?
		bool m_actorAttraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)			//Are the actors attracted?
		int m_lifeSpan=3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)			
		UProjectileMovementComponent* m_projectileComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_sphereCol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_mesh;

	UPROPERTY(BlueprintReadWrite)
		UParticleSystem* m_attractionParticle;
};
