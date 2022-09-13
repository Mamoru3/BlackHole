// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "UExplosion_Projectile.generated.h"

UCLASS(Blueprintable)
class GRAVITY_BALL_API AUExplosion_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUExplosion_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)		//Stop projectile
		void setNewVel();

	UFUNCTION(BlueprintCallable)
		void castExplToActors();		//Cast the explosion

	UPROPERTY(EditAnywhere, BlueprintReadWrite)		//Expl proj life span
		int m_ExplLifeSpan = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)		//Boolean to track if the projectile is stopped
		bool m_projStop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProjectileMovementComponent* m_ProjComp;

	UPROPERTY(BlueprintReadWrite)
		UParticleSystem* m_explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_sphereCol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_mesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
