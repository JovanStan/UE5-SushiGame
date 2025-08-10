

#include "SushiTable.h"

#include "SushiCharacter.h"
#include "SushiPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "UI/OrderWidget.h"

class ASushiPlayerState;

ASushiTable::ASushiTable()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	OrderWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OrderWidgetComponent"));
	OrderWidgetComponent->SetupAttachment(RootComponent);
	
	OrderWidgetComponent->SetDrawSize(FVector2D(200.f, 100.f));
	OrderWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 150.f)); 
	OrderWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	OrderWidgetComponent->SetVisibility(false);
}

void ASushiTable::BeginPlay()
{
	Super::BeginPlay();

	if (OrderWidgetClass)
	{
		OrderWidgetComponent->SetWidgetClass(OrderWidgetClass);
	}

	if (OrderWidgetComponent)
	{
		OrderWidgetInstance = Cast<UOrderWidget>(OrderWidgetComponent->GetUserWidgetObject());
		if (OrderWidgetInstance)
		{
			OrderWidgetComponent->SetVisibility(false);
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(OrderTimerHandle, this, &ASushiTable::GenerateOrder, 5.f, false);
}

void ASushiTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASushiTable::GenerateOrder()
{
	bHasActiveOrder = true;
	
	TArray<FName> SushiTypes = { "SushiRoll", "Nigiri", "Tempura" };
	OrderedSushi = SushiTypes[FMath::RandRange(0, SushiTypes.Num() - 1)];
	
	RemainingOrderTime = OrderTimeLimit;
	
	if (OrderWidgetComponent)
	{
		OrderWidgetComponent->SetVisibility(true);
	}

	if (OrderWidgetInstance)
	{
		OrderWidgetInstance->UpdateOrder(FText::FromName(OrderedSushi), RemainingOrderTime);
	}
	
	// Start timer for this order
	GetWorld()->GetTimerManager().SetTimer(OrderTimerHandle, this, &ASushiTable::FailOrder, OrderTimeLimit, false);

	GetWorld()->GetTimerManager().SetTimer(OrderUpdateTimerHandle, this, &ASushiTable::UpdateOrderTimer, 0.1f, true);
}

void ASushiTable::UpdateOrderTimer()
{
	RemainingOrderTime -= 0.1f;
	if (RemainingOrderTime < 0.f)
		RemainingOrderTime = 0.f;

	if (OrderWidgetInstance)
	{
		OrderWidgetInstance->UpdateOrder(FText::FromName(OrderedSushi), RemainingOrderTime);
	}
}

void ASushiTable::FailOrder()
{
	if (bHasActiveOrder)
	{
		bHasActiveOrder = false;
		UE_LOG(LogTemp, Warning, TEXT("Order failed! Game Over here if needed."));

		if (OrderWidgetComponent)
		{
			OrderWidgetComponent->SetVisibility(false);
		}

		if (OrderWidgetInstance)
		{
			OrderWidgetInstance->ClearOrder();
		}

		GetWorld()->GetTimerManager().ClearTimer(OrderUpdateTimerHandle);
	}
}

bool ASushiTable::TryDeliverSushi(ASushiCharacter* Player)
{
	if (!bHasActiveOrder || !Player) return false;

	if (Player->GetHeldSushiName() == OrderedSushi)
	{
		bHasActiveOrder = false;
		GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(OrderUpdateTimerHandle);
		
		if (OrderWidgetComponent)
		{
			OrderWidgetComponent->SetVisibility(false);
		}

		if (OrderWidgetInstance)
		{
			OrderWidgetInstance->ClearOrder();
		}
		
		Player->ClearHeldSushi();
		
		if (ASushiPlayerState* PS = Player->GetPlayerState<ASushiPlayerState>())
		{
			PS->AddScore(10); 
		}
		UE_LOG(LogTemp, Log, TEXT("Order completed!"));

		// Queue next order after delay
		GetWorld()->GetTimerManager().SetTimer(OrderTimerHandle, this, &ASushiTable::GenerateOrder, 5.f, false);

		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Wrong sushi!"));
	return false;
}

