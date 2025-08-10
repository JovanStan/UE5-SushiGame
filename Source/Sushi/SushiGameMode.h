
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SushiGameMode.generated.h"


UCLASS(abstract)
class ASushiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASushiGameMode();

	virtual void BeginPlay() override;

};



