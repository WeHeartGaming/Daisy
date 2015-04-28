// 2015, WeHeartGaming

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "DCharacterMovementComponent.generated.h"

/**
*
*/
UCLASS()
class DAISY_API UDCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

		virtual float GetMaxSpeed() const override;
};
