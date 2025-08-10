

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
		CookwareState = ECookwareState::Cutting;
		Player->StartCutting(this);
		SpawnCuttingUI();
		break;

	case ECookwareState::Cutting:
		// Wait for player to finish cutting first (do nothing)
		//Player->NotifyCannotProceedYet();
		break;

	case ECookwareState::CuttingDone:
		CookwareState = ECookwareState::Rolling;
		Player->StartRolling(this);
		SpawnRollingUI();
		break;

	case ECookwareState::Rolling:
		// Wait for rolling to finish
		//Player->NotifyCannotProceedYet();
		break;

	case ECookwareState::RollingDone:
		CookwareState = ECookwareState::Idle;
		Player->TakeSushi();
		//Player->ClearCookware();
		break;

	}
}