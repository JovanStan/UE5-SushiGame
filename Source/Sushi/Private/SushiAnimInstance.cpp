
#include "SushiAnimInstance.h"

#include "SushiCharacter.h"

void USushiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SushiCharacter = Cast<ASushiCharacter>(TryGetPawnOwner());
}

void USushiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (SushiCharacter == nullptr)
	{
		SushiCharacter = Cast<ASushiCharacter>(TryGetPawnOwner());
	}

	if (SushiCharacter)
	{
		Speed = SushiCharacter->GetVelocity().Length();
	}
}
