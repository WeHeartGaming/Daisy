// 2015, WeHeartGaming

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "DPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DAISY_API ADPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	virtual void UpdateCamera(float DeltaTime) override;
	
public:
	ADPlayerCameraManager(const class FObjectInitializer& ObjectInitializer);

	float normalFOV;
	float zoomFOV;

	float yawMin;
	float defaultYawMin;
	float yawMax;
	float defaultYawMax;
};
