
#include "SushiGameMode.h"

#include "SushiPlayerState.h"


ASushiGameMode::ASushiGameMode()
{
    PlayerStateClass = ASushiPlayerState::StaticClass();
}

void ASushiGameMode::BeginPlay()
{
    Super::BeginPlay();

}

