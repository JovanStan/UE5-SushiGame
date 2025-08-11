

#include "SushiCookware.h"

#include "SushiCharacter.h"
#include "Net/UnrealNetwork.h"


ASushiCookware::ASushiCookware()
{
	PrimaryActorTick.bCanEverTick = false;
	CookwareState = ECookwareState::Idle;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SushiSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SushiSpawnLocation"));
	SushiSpawnLocation->SetupAttachment(Mesh);
	bReplicates = true;
	AActor::SetReplicateMovement(false);
}

void ASushiCookware::BeginPlay()
{
	Super::BeginPlay();
}

void ASushiCookware::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASushiCookware::Interact(ASushiCharacter* Player)
{
	if (!Player) return;

	switch (CookwareState)
	{
	case ECookwareState::Idle:
		Player->StartCutting(this);
		SpawnCuttingUI();
		break;
	case ECookwareState::CuttingDone:
		Player->StartRolling(this);
		SpawnRollingUI();
		break;
	case ECookwareState::RollingDone:
		CookwareState = ECookwareState::Idle;
		Player->TakeSushi();
		break;
	default: ;
	}
}
