
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SushiPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, NewScore);

UCLASS()
class SUSHI_API ASushiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASushiPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerScore, BlueprintReadOnly)
	int32 PlayerScore;

	UFUNCTION()
	void OnRep_PlayerScore();

	void AddScore(int32 ScoreToAdd);

	UPROPERTY(BlueprintAssignable, Category="Score")
	FOnScoreChangedSignature OnScoreChanged;
};
