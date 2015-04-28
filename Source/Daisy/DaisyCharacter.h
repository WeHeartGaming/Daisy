// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DaisyCharacter.generated.h"

UCLASS()
class DAISY_API ADaisyCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FirstPersonCamera;
	bool isInFirstPerson;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float sprintModifier;

	FRotator previousRotation;

public:
	// Sets default values for this character's properties
	ADaisyCharacter(const class FObjectInitializer& ObjectInitializer);

	/* MOVEMENT */
	virtual void MoveForward(float Val);
	virtual void MoveRight(float Val);
	void OnSprintStart();
	void OnSprintStop();
	void OnFreelookStart();
	void OnFreelookStop();
	void OnZoomStart();
	void OnZoomStop();
	void OnCameraToggle();
	
	float GetSprintModifier() const;

	UPROPERTY(Transient, Replicated)
	bool bWantToSprint;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsSprinting() const;

	bool isFreelooking;
	bool isZooming;

	// Client/local
	void SetSprinting(bool newSprinting);
	// Server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(bool newSprinting);

	/* Player vars */
	UFUNCTION(BlueprintCallable, Category = "PlayerVars")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerVars")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerVars")
	bool IsAlive() const;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector boomSocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector boomTargetOffset;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerVars", Replicated)
	float health;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
};
