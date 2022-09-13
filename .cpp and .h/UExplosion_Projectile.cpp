// Fill out your copyright notice in the Description page of Project Settings.


#include "UExplosion_Projectile.h"

// Sets default values
AUExplosion_Projectile::AUExplosion_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_ProjComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));		//Create the different subobjects 
	m_sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> explosionParticle(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));	//Use constructor helpers to load elements
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/FirstPerson/Meshes/ExplosionProjectileMesh.ExplosionProjectileMesh'"));

	m_ProjComp->InitialSpeed = 1000.f;		//Initial speed set
	m_ProjComp->MaxSpeed = 2000.f;			//Max speed set
	m_ProjComp->ProjectileGravityScale = 0;			//Projectile gravity disabled, projectile floats

	m_explosion = explosionParticle.Object;		//Initialize reference to the just loaded particle
	//m_projectile_component.a
	//m_projectile_component->ProjectileGravityScale=0;
	m_sphereCol->InitSphereRadius(20.f);		//Sphere radius collision set
	m_sphereCol->BodyInstance.SetCollisionProfileName("Projectile");

	RootComponent = m_sphereCol;		//Sphere collision as root component

	UStaticMesh* Asset = MeshAsset.Object;		//Initialize reference to the just loaded mesh
	m_mesh->SetStaticMesh(Asset);		//Set mesh
	m_mesh->SetupAttachment(m_sphereCol);		//mesh attached to sphere collision (root component)
	m_mesh->SetWorldScale3D(FVector(0.4, 0.4, 0.4));		//Size of projectile

}

// Called when the game starts or when spawned
void AUExplosion_Projectile::BeginPlay()
{

	SetLifeSpan(m_ExplLifeSpan);		//Life span 
	Super::BeginPlay();
}

// Called every frame
void AUExplosion_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();		//Get player location
	FVector projLoc = AUExplosion_Projectile::GetActorLocation();		//Get projectile location
	FVector distToPlayer = projLoc - playerLoc;			//Distance from projectile to player
	float maxDist = 1300;			//Distance at which the projectile stops
	if (abs(FVector::Dist(projLoc, playerLoc)) > maxDist || AUExplosion_Projectile::GetVelocity() == FVector(0))			//TO CHANGE, CHECK POSITIONS BETWEEN PROJECTILE AND MAIN PLAYER.
	{
		m_projStop = true;		//Boolean for projectile to stop
		setNewVel();			//Sets new velocity
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_explosion, projLoc);			//Spawn emitter
	}
}

void AUExplosion_Projectile::setNewVel()
{
	if (m_projStop == true)		//If bool is true
	{
		m_ProjComp->SetVelocityInLocalSpace(FVector(0));		//Stops bullet
		castExplToActors();			//Explosion is cast
	}
}


void AUExplosion_Projectile::castExplToActors()
{
	TArray<AActor*> FoundActors;															  //Array to detect actors around the projectile
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);	  		//Get all actors in the scene

	for (auto& x : FoundActors)
	{
		UPrimitiveComponent* primActor_;			//Found actors
		primActor_ = Cast<UPrimitiveComponent>(x->GetRootComponent());
		if (primActor_ != NULL)
		{
			FVector propLoc = primActor_->GetComponentLocation();		//found actor location
			FVector projLoc = AUExplosion_Projectile::GetActorLocation();		//projectile location
			FVector distToProj = propLoc - projLoc;		//Dist from actor to projectile
			float attractionDist = 900;			//distance for attracting objects, if distance is higher than 900, don't attract
			if (abs(FVector::Dist(propLoc, projLoc)) < attractionDist)		//if the objects are close
			{
				primActor_->AddImpulse(FVector(distToProj) * primActor_->GetMass() / 5);		//Add impulse of explosion to actors
			}

		}
	}
}

