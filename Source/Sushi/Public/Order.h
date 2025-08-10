
#pragma once

#include "CoreMinimal.h"
#include "Order.generated.h"

USTRUCT(BlueprintType)
struct FActiveOrder
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName OrderName = NAME_None;

	UPROPERTY(BlueprintReadWrite)
	float TimeRemaining = 30.f;

	UPROPERTY(BlueprintReadWrite)
	int32 TableId = -1;

	UPROPERTY(BlueprintReadWrite)
	bool bActive = true;
};