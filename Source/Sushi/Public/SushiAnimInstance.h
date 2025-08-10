
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SushiAnimInstance.generated.h"

class ASushiCharacter;

UCLASS()
class SUSHI_API USushiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ASushiCharacter> SushiCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;
};
