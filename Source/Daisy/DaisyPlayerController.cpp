// 2015, WeHeartGaming

#include "Daisy.h"
#include "DaisyPlayerController.h"
#include "DPlayerCameraManager.h"

ADaisyPlayerController::ADaisyPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ADPlayerCameraManager::StaticClass();
}
