// 2015, WeHeartGaming

#include "Daisy.h"
#include "DPlayerCameraManager.h"
#include "DaisyCharacter.h"

ADPlayerCameraManager::ADPlayerCameraManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	normalFOV = 85.0f;
	zoomFOV = 50.0f;

	ViewPitchMin = -80.0f;
	ViewPitchMax = 80.0f;

	defaultYawMin = ViewYawMin;
	defaultYawMax = ViewYawMax;

	yawMin = -170.0f;
	yawMax = 170.0f;

	bAlwaysApplyModifiers = true;

	SetFOV(normalFOV);
}

void ADPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	ADaisyCharacter* owner = Cast<ADaisyCharacter>(PCOwner->GetPawn());

	if (owner)
	{
		bool isFreelooking = owner->isFreelooking;
		bool isZooming = owner->isZooming;

		if (isFreelooking)
		{
			ViewYawMin = owner->GetActorRotation().Yaw + yawMin;
			ViewYawMax = owner->GetActorRotation().Yaw + yawMax;
		}
		else
		{
			ViewYawMin = defaultYawMin;
			ViewYawMax = defaultYawMax;
		}

		if (isZooming)
		{
			DefaultFOV = FMath::FInterpTo(DefaultFOV, zoomFOV, DeltaTime, 10.0f);
			SetFOV(DefaultFOV);
		}
		else
		{
			DefaultFOV = FMath::FInterpTo(DefaultFOV, normalFOV, DeltaTime, 10.0f);
			SetFOV(DefaultFOV);
		}
	}

	/*
		TODO: Reset camera to forward rotation of character
	*/

	Super::UpdateCamera(DeltaTime);
}
