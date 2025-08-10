
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SushiUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SUSHI_API USushiUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetInfoText(const FText& NewText);

	UFUNCTION(BlueprintCallable)
	void SetScoreText(int32 NewScore);

	UPROPERTY(meta = (BindWidget))
	 TObjectPtr<UTextBlock> InfoText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

private:
	FTimerHandle ClearTextTimerHandle;

	UFUNCTION()
	void ClearInfoText();
};
