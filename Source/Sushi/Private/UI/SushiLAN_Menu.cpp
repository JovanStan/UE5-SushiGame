

#include "UI/SushiLAN_Menu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void USushiLAN_Menu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
	
	HostButton->OnClicked.AddDynamic(this, &USushiLAN_Menu::HostButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &USushiLAN_Menu::JoinButtonClicked);
}

void USushiLAN_Menu::HostButtonClicked()
{
	const FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, HostingLevel, true, TEXT("listen"));
}

void USushiLAN_Menu::JoinButtonClicked()
{
	const FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);

	const FString Address = IpAddressTextBlock->GetText().ToString();
	UGameplayStatics::OpenLevel(this, *Address);
}