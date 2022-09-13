// Fill out your copyright notice in the Description page of Project Settings.


#include "UFirstPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AUFirstPersonCharacter::AUFirstPersonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// set our turn rates for input
		// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(200.0f, 0.0f, 10.0f);

	//m_Mesh2P->SetupAttachment(FirstPersonCameraComponent);
}

// Called when the game starts or when spawned
void AUFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

}

// Called every frame
void AUFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManaRegen();
}

// Called to bind functionality to input
void AUFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);


	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AUFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUFirstPersonCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUFirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUFirstPersonCharacter::LookUpAtRate);
}
//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

void AUFirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUFirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUFirstPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUFirstPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void AUFirstPersonCharacter::changeBullCountDown()		//If mouse wheel goes down, decrease bullet counter
{
	m_changeBulletVar--;
	if (m_changeBulletVar >= 2)		//If variable goes beyond the last projectile, the variable is stuck at the last projectile
		m_changeBulletVar = 2;

	if (m_changeBulletVar <= 0)			//If variable goes beyond the first projectile, the variable is stuck at the first projectile
		m_changeBulletVar = 0;
}
void AUFirstPersonCharacter::changeBullCountUp()		//If mouse wheel goes down, increase bullet counter
{
	m_changeBulletVar++;
	if (m_changeBulletVar > 2)			 //If variable goes beyond the last projectile, the variable is stuck at the last projectile
		m_changeBulletVar = 2;
	if (m_changeBulletVar < 0)
		m_changeBulletVar = 0;			 	//If variable goes beyond the first projectile, the variable is stuck at the first projectile
}

void AUFirstPersonCharacter::spawnProj()
{
	if (m_projectileSpawnable == true)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + SpawnRotation.RotateVector(GunOffset));
		FActorSpawnParameters SpawnInfo;
		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		if (m_changeBulletVar == 0 && m_mana >= m_GravHoleCost)		//If the gravity bullet is selected and the player has enough mana
		{
			GetWorldTimerManager().SetTimer(Cooldown, this, &AUFirstPersonCharacter::TimerFunc, m_gravCooldown, false);			//Timer for cooldown
			GetWorld()->SpawnActor<AUGravity_Projectile>(SpawnLocation, SpawnRotation, SpawnInfo);				//spawn gravity projectile
			m_projectileSpawnable = false;	 //Projectile is not spawnable until cooldown is over	
			m_mana -= m_GravHoleCost;		//mana minus cost for casting

			if (FireSound != nullptr)		// try and play the sound if specified
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}
		if (m_changeBulletVar == 1 && m_mana >= m_ExplCost)			//If the explosion bullet is selected and the player has enough mana
		{
			GetWorldTimerManager().SetTimer(Cooldown, this, &AUFirstPersonCharacter::TimerFunc, m_explCooldown, false);		 	//Timer for cooldown
			GetWorld()->SpawnActor<AUExplosion_Projectile>(SpawnLocation, SpawnRotation, SpawnInfo);			 //spawn explosion projectile
			m_projectileSpawnable = false;			//Projectile is not spawnable until cooldown is over
			m_mana -= m_ExplCost;		//mana minus cost for casting

			if (FireSound != nullptr)		// try and play the sound if specified
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}
		if (m_changeBulletVar == 2 && m_mana >= m_zeroGravCost)			//If the zero-gravity bullet is selected and the player has enough mana
		{
			GetWorldTimerManager().SetTimer(Cooldown, this, &AUFirstPersonCharacter::TimerFunc, m_zeroGravCooldown, false);		  	//Timer for cooldown
			GetWorld()->SpawnActor<AUFloatingBullet>(SpawnLocation, SpawnRotation, SpawnInfo);					  //spawn zero-grav projectile
			m_projectileSpawnable = false;																		  //Projectile is not spawnable until cooldown is over
			m_mana -= m_zeroGravCost;		//mana minus cost for casting

			if (FireSound != nullptr)		// try and play the sound if specified
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}
	}
}

void AUFirstPersonCharacter::TimerFunc()
{
	m_projectileSpawnable = true;		//Project is spawnable again
}

void AUFirstPersonCharacter::ManaRegen()
{
	m_mana += m_manaRechargeRate;		//Mana is recharged every second
	if (m_mana >= 100.f)			//Mana doesn't go over 100
		m_mana = 100.f;
}
