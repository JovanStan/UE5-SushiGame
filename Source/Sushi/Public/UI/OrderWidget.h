
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrderWidget.generated.h"


UCLASS()
class SUSHI_API UOrderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateOrder(const FText& OrderName, float TimeLeft);
    
	UFUNCTION(BlueprintImplementableEvent)
	void ClearOrder();
};
