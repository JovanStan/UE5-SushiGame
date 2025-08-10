

#include "SushiCookware.h"

#include "SushiCharacter.h"


ASushiCookware::ASushiCookware()
{
	PrimaryActorTick.bCanEverTick = false;
	CookwareState = ECookwareState::Idle;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SushiSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SushiSpawnLocation"));
	SushiSpawnLocation->SetupAttachment(Mesh);
}

void ASushiCookware::BeginPlay()
{
	Super::BeginPlay();
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