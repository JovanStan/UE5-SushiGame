
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SushiCookware.generated.h"

class ASushiCharacter;

UENUM(BlueprintType)
enum class ECookwareState : uint8
{
	Idle,
	CuttingDone,
	RollingDone
};

UCLASS()
class SUSHI_API ASushiCookware : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnCuttingUI();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnRollingUI();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CuttingTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollingTime = 3.f;
	
	ASushiCookware();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sushi")
	ECookwareState CookwareState;

	void Interact(ASushiCharacter* Player);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sushi")
	FName SushiName;

	FORCEINLINE USceneComponent* GetSushiSpawnLocation() const { return SushiSpawnLocation; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sushi", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sushi", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SushiSpawnLocation;
};
