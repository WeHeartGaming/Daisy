// 2015, WeHeartGaming

#include "Daisy.h"
#include "DCharacterMovementComponent.h"
#include "DaisyCharacter.h"


float UDCharacterMovementComponent::GetMaxSpeed() const
{
	float maxSpeed = Super::GetMaxSpeed();

	const ADaisyCharacter* owner = Cast<ADaisyCharacter>(PawnOwner);
	if (owner)
	{
		if (owner->IsSprinting())
			maxSpeed *= owner->GetSprintModifier();
	}

	return maxSpeed;
}

