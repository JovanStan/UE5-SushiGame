
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SushiLAN_Menu.generated.h"


class UButton;
class UEditableTextBox;

UCLASS()
class SUSHI_API USushiLAN_Menu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> IpAddressTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> HostButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> JoinButton;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> HostingLevel;

	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();
};
