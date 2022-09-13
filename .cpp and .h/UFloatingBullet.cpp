// Fill out your copyright notice in the Description page of Project Settings.


#include "UFloatingBullet.h"

// Sets default values
AUFloatingBullet::AUFloatingBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_ProjComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));		//Create the different subobjects
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));
	m_sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/FirstPerson/Meshes/ZeroGravMesh.ZeroGravMesh'"));				//Use constructor helper to load mesh

	m_ProjComp->InitialSpeed = 1000.f;		//Initial speed of floating bullet
	m_ProjComp->MaxSpeed = 20000.f;
	m_ProjComp->ProjectileGravityScale = 0;			//Gravity is zero for the bullet (projectile floats)

	m_sphereCol->InitSphereRadius(20.f);		//Sphere radius 
	m_sphereCol->BodyInstance.SetCollisionProfileName("Projectile");		//

	RootComponent = m_sphereCol;		//Collision sphere is the root component

	UStaticMesh* Asset = MeshAsset.Object;		//Initialize reference to the just loaded mesh
	m_mesh->SetStaticMesh(Asset);		//Set mesh
	m_mesh->SetupAttachment(m_sphereCol);		//Attach mesh to root component (sphere collision)
	m_mesh->SetWorldScale3D(FVector(0.4, 0.4, 0.4));		//Initial size

}

// Called when the game starts or when spawned
void AUFloatingBullet::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(m_floatingLifeSpan);			//Life span is 8
}

// Called every frame
void AUFloatingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();		//Get player location	
	FVector projLoc = AUFloatingBullet::GetActorLocation();			//Get projectile location
	FVector distToPlayer = projLoc - playerLoc;			//get distance from player to bullet 
	float maxDist = 1300;		//Distance at which the bullet stop
	if (abs(FVector::Dist(projLoc, playerLoc)) >maxDist || AUFloatingBullet::GetVelocity() == FVector(0))			//TO CHANGE, CHECK POSITIONS BETWEEN PROJECTILE AND MAIN PLAYER.
	{
		m_projStop = true;		//Projectile stops bool
		setZeroVel();			//stop bullet and make actor float
	}

}

// Called every frame
void AUFloatingBullet::zeroGravity()
{
	TArray<AActor*> FoundActors;			//Array to detect actors around the projectile
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);			//Get all actors in the scene

	for (auto& x : FoundActors)
	{
		UPrimitiveComponent* primActor_;		//Found actors
		primActor_ = Cast<UPrimitiveComponent>(x->GetRootComponent());
		if (primActor_ != NULL)
		{
			FVector propLoc = primActor_->GetComponentLocation();	  //found actor location
			FVector projLoc = AUFloatingBullet::GetActorLocation();	  	//projectile location


			float attractionDist = 900;			//distance for attracting objects, if distance is higher than 900, don't attract
			if (abs(FVector::Dist(propLoc, projLoc)) < attractionDist&&(AUFloatingBullet::GetLifeSpan()<= m_floatingLifeSpan && AUFloatingBullet::GetLifeSpan() >1.5))		//if the objects are close, and the bullet is not disappear then attract the projectile
			{
				primActor_->SetEnableGravity(false);			//Disable gravity
				
				primActor_->AddImpulse(FVector(0,0,2000));		//Make objects float

			}
			else if(AUFloatingBullet::GetLifeSpan() < 1)		//If the projectile is going to disappear
			{

				primActor_->SetEnableGravity(true);		//Enable gravity again
			}
		}
	}
}



// Called every frame
void AUFloatingBullet::setZeroVel()
{
	if (m_projStop == true)	//If projectile stop boolean
	{
		m_ProjComp->SetVelocityInLocalSpace(FVector(0));		//Stop the projectile
		zeroGravity();			//apply zero gravity
	}
}