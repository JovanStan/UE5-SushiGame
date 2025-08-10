
#include "SushiPlayerState.h"

#include "Net/UnrealNetwork.h"

ASushiPlayerState::ASushiPlayerState()
{
	PlayerScore = 0;
}

void ASushiPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASushiPlayerState, PlayerScore);
}

void ASushiPlayerState::OnRep_PlayerScore()
{
	OnScoreChanged.Broadcast(PlayerScore);
}

void ASushiPlayerState::AddScore(int32 ScoreToAdd)
{
	if (HasAuthority())
	{
		PlayerScore += ScoreToAdd;
		OnScoreChanged.Broadcast(PlayerScore);
	}
}

