// Fill out your copyright notice in the Description page of Project Settings.

#include "Daisy.h"
#include "DaisyCharacter.h"
#include "DCharacterMovementComponent.h"


// Sets default values
ADaisyCharacter::ADaisyCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	UCharacterMovementComponent* MoveComponent = GetCharacterMovement();
	MoveComponent->GravityScale = 1.5f;
	MoveComponent->bCanWalkOffLedgesWhenCrouching = true;
	MoveComponent->MaxWalkSpeedCrouched = 200.0f;

	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0f, 40.0f, 50.0f);
	CameraBoom->TargetOffset = FVector(0.0f, 0.0f, 45.0f);
	CameraBoom->AttachParent = GetRootComponent();

	ThirdPersonCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->AttachParent = CameraBoom;

	isFreelooking = false;
	isZooming = false;

	sprintModifier = 1.6f;
	health = 100;
}

void ADaisyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// DO HUNGER STUFF HERE AND MAYBE OTHER DETERIORATION BASED STUFF
}

// Called when the game starts or when spawned
void ADaisyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADaisyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bWantToSprint && !IsSprinting())
		SetSprinting(true);
}

// Called to bind functionality to input
void ADaisyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ADaisyCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADaisyCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ADaisyCharacter::OnSprintStart);
	InputComponent->BindAction("Sprint", IE_Released, this, &ADaisyCharacter::OnSprintStop);
	InputComponent->BindAction("Freelook", IE_Pressed, this, &ADaisyCharacter::OnFreelookStart);
	InputComponent->BindAction("Freelook", IE_Released, this, &ADaisyCharacter::OnFreelookStop);
	InputComponent->BindAction("Zoom", IE_Pressed, this, &ADaisyCharacter::OnZoomStart);
	InputComponent->BindAction("Zoom", IE_Released, this, &ADaisyCharacter::OnZoomStop);
	//InputComponent->BindAction("Crouch", IE_Pressed, this, &ADaisyCharacter::OnCrouchStart);
	//InputComponent->BindAction("Crouch", IE_Released, this, &ADaisyCharacter::OnCrouchStop);
}

void ADaisyCharacter::MoveForward(float Val) 
{
	if (Controller && Val != 0.0f)
	{
		const FRotator Rotation = GetActorRotation();

		if (!isFreelooking)
		{
			const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

			AddMovementInput(Direction, Val);
		}
		else
		{
			const FRotator YawRotation(0, previousRotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(previousRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(Direction, Val);
		}

	}
}

void ADaisyCharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		const FRotator Rotation = GetActorRotation();

		if (!isFreelooking)
		{
			const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(Direction, Val);
		}
		else
		{
			const FRotator YawRotation(0, previousRotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(previousRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(Direction, Val);
		}
	}
}

void ADaisyCharacter::SetSprinting(bool newSprinting)
{
	bWantToSprint = newSprinting;

	if (Role < ROLE_Authority)
		ServerSetSprinting(newSprinting);
}

void ADaisyCharacter::OnSprintStart()
{
	SetSprinting(true);
}

void ADaisyCharacter::OnSprintStop()
{
	SetSprinting(false);
}

void ADaisyCharacter::OnFreelookStart()
{
	isFreelooking = true;

	bUseControllerRotationYaw = false;
	CharacterMovement->bOrientRotationToMovement = false;

	const FRotator Rotation = GetActorRotation();
	previousRotation = Rotation;
}

void ADaisyCharacter::OnFreelookStop()
{
	isFreelooking = false;

	bUseControllerRotationYaw = true;
	CharacterMovement->bOrientRotationToMovement = true;
}

void ADaisyCharacter::OnZoomStart()
{
	isZooming = true;
}

void ADaisyCharacter::OnZoomStop()
{
	isZooming = false;
}

void ADaisyCharacter::ServerSetSprinting_Implementation(bool newSprinting)
{
	SetSprinting(newSprinting);
}

bool ADaisyCharacter::ServerSetSprinting_Validate(bool newSprinting)
{
	return true;
}

bool ADaisyCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
		return false;

	return bWantToSprint && !GetVelocity().IsZero() && (GetVelocity().GetSafeNormal2D() | GetActorRotation().Vector()) > 0.1;
}

float ADaisyCharacter::GetHealth() const
{
	return health;
}

float ADaisyCharacter::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<ADaisyCharacter>()->health;
}

float ADaisyCharacter::GetSprintModifier() const
{
	return sprintModifier;
}

void ADaisyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ADaisyCharacter, bWantToSprint, COND_SkipOwner);

	DOREPLIFETIME(ADaisyCharacter, health);	
}

bool ADaisyCharacter::IsAlive() const
{
	return health > 0;
}

float ADaisyCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (health <= 0.0f)
		return 0.0f;

	const float actualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (actualDamage > 0.0f)
	{
		health -= actualDamage;

		if (health <= 0)
		{
				// DEAD
		}
		else
		{

		}
	}

	return actualDamage;
}