// Fill out your copyright notice in the Description page of Project Settings.

#include "UGravity_Projectile.h"

// Sets default values
AUGravity_Projectile::AUGravity_Projectile()
{
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_projectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));			//Create subobjects for the proj comp
	m_sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));				//Create subobjects for the sphere collision
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));							//Create subobjects for the mesh
	static ConstructorHelpers::FObjectFinder<UParticleSystem> attractionPartTemp(TEXT("ParticleSystem'/Game/FirstPersonBP/Blueprints/My_Explosion_Purple.My_Explosion_Purple'"));		//Load particle 
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/FirstPerson/Meshes/FirstPersonProjectileMesh.FirstPersonProjectileMesh'"));			//Load mesh 

	m_attractionParticle = attractionPartTemp.Object;		//Initialize reference to the just loaded particle
	//m_projectile_component.a
	//m_projectile_component->ProjectileGravityScale=0;

	m_sphereCol->InitSphereRadius(20.0f);			//Radius of sphere collision
	m_sphereCol->BodyInstance.SetCollisionProfileName("Projectile");


	RootComponent = m_sphereCol;		//Root is the sphere collision component
	m_projectileComponent->InitialSpeed = 1000.f;		//Initial speed of projectile
	m_projectileComponent->MaxSpeed = 2000.f;			//Max speed of projectile
	m_projectileComponent->ProjectileGravityScale = 0;			//Zero gravity on projectile (it floats)
	UStaticMesh* Asset = MeshAsset.Object;			//Initialize reference to the just loaded mesh

	m_mesh->SetStaticMesh(Asset);			//Set mesh 
	m_mesh->SetupAttachment(m_sphereCol);		//Attached to the sphere collision (root component)
	m_mesh->SetWorldScale3D(FVector(0.4, 0.4, 0.4));		//Initial size 
	

}

// Called when the game starts or when spawned
void AUGravity_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(m_lifeSpan);			//Life span of projectile
}

// Called every frame
void AUGravity_Projectile::Tick(float DeltaTime)		//Called every frame
{
	Super::Tick(DeltaTime);

	FVector playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();		//Calculate gets the player location
	FVector projLoc = AUGravity_Projectile::GetActorLocation();				//Gets the projectile location
	FVector distToPlayer = projLoc - playerLoc;				//Gets the distance from the projectile to the player

	float maxDist = 1300;			//Distance from projectile to 
	if (abs(FVector::Dist(projLoc, playerLoc)) > maxDist || AUGravity_Projectile::GetVelocity() == FVector(0))			//TO CHANGE, CHECK POSITIONS BETWEEN PROJECTILE AND MAIN PLAYER.
	{
		m_projectileStop = true;	//Bool to track if the projectile if stopped
		stopBullet();				//Stop the bullet
	}
}

void AUGravity_Projectile::castAttractionToObjects()
{
	TArray<AActor*> FoundActors;															//Array to detect actors around the projectile
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);			//Get all actors in the scene
	for (auto& x : FoundActors)
	{
		UPrimitiveComponent* primActor_;			//Found actors
		primActor_ = Cast<UPrimitiveComponent>(x->GetRootComponent());
		if (primActor_ != NULL)
		{
			FVector propLoc = primActor_->GetComponentLocation();		//found actor location
			FVector projLoc = AUGravity_Projectile::GetActorLocation();		//projectile location
			FVector distToProj = projLoc - propLoc;		//Dist from actor to projectile
			float attractionDist = 900;			//distance for attracting objects, if distance is higher than 900, don't attract
			if (abs(FVector::Dist(propLoc, projLoc)) < attractionDist)		//if the objects are close
			{
				primActor_->AddImpulse(distToProj * primActor_->GetMass());		//Attract them 
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_attractionParticle, projLoc);		//Spawn particles
			}
		}
	}
}

void AUGravity_Projectile::stopBullet()		
{
	if (m_projectileStop == true)											//If the boolean is true
	{
		m_projectileComponent->SetVelocityInLocalSpace(FVector(0));		//Stop the bullet
		castAttractionToObjects();											//Attract bullets
	}
}