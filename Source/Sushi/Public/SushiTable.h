
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SushiTable.generated.h"

class UWidgetComponent;
class UOrderWidget;
class ASushiCharacter;
class USphereComponent;

UCLASS()
class SUSHI_API ASushiTable : public AActor
{
	GENERATED_BODY()
	
public:	
	ASushiTable();
	bool TryDeliverSushi(ASushiCharacter* Player);
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> OrderWidgetClass;
	UPROPERTY()
	TObjectPtr<UOrderWidget> OrderWidgetInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	TObjectPtr<UWidgetComponent> OrderWidgetComponent;

	FTimerHandle OrderUpdateTimerHandle;
	
	float RemainingOrderTime;
	void UpdateOrderTimer();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasActiveOrder = false;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FName OrderedSushi;
	
	FTimerHandle OrderTimerHandle;
	UPROPERTY(EditAnywhere)
	float OrderTimeLimit = 30.f;
	
	UFUNCTION()
	void GenerateOrder();
	UFUNCTION()
	void FailOrder();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOverScreen();
};
