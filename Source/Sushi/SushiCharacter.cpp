
#include "SushiCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SushiCookware.h"
#include "SushiPlayerState.h"
#include "SushiTable.h"
#include "Blueprint/UserWidget.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/SushiUserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASushiCharacter::ASushiCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	ACharacter::SetReplicateMovement(true);
	HeldSushiName = "";
	bSushiInHand = false;
}

void ASushiCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UserWidgetClass)
	{
		UserWidget = CreateWidget<USushiUserWidget>(GetWorld(), UserWidgetClass);
		if (UserWidget)
		{
			UserWidget->AddToViewport();
		}
	}

	if (ASushiPlayerState* PS = GetPlayerState<ASushiPlayerState>())
	{
		PS->OnScoreChanged.AddDynamic(this, &ASushiCharacter::HandleScoreChanged);
		UserWidget->SetScoreText(PS->PlayerScore);
	}
}

void ASushiCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASushiCharacter, bSushiInHand);
	DOREPLIFETIME(ASushiCharacter, HeldSushiName);
	DOREPLIFETIME(ASushiCharacter, SushiActor);
}

void ASushiCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (ASushiPlayerState* PS = GetPlayerState<ASushiPlayerState>())
	{
		PS->OnScoreChanged.RemoveDynamic(this, &ASushiCharacter::HandleScoreChanged);

		PS->OnScoreChanged.AddDynamic(this, &ASushiCharacter::HandleScoreChanged);
		if (UserWidget)
		{
			UserWidget->SetScoreText(PS->PlayerScore);
		}
	}
}

void ASushiCharacter::HandleScoreChanged(int32 NewScore)
{
	if (UserWidget)
	{
		UserWidget->SetScoreText(NewScore);
	}
}


void ASushiCharacter::StartCutting(ASushiCookware* Cookware)
{
	bIsBusy = true;
	CurrentCookware = Cookware;
	SetIgnoreLookInputAndMovementMode(true, MOVE_None);
	
	
	GetWorldTimerManager().SetTimer(CuttingTimerHandle, this, &ASushiCharacter::FinishCutting, CurrentCookware->CuttingTime, false);
}

void ASushiCharacter::FinishCutting()
{
	bIsBusy = false;
	SetIgnoreLookInputAndMovementMode(false, MOVE_Walking);

	if (CurrentCookware)
		CurrentCookware->CookwareState = ECookwareState::CuttingDone;
	
	UserWidget->SetInfoText(FText::FromString(TEXT("Cutting Finished. You can roll sushi now")));
}

void ASushiCharacter::StartRolling(ASushiCookware* Cookware)
{
	bIsBusy = true;
	CurrentCookware = Cookware;
	SetIgnoreLookInputAndMovementMode(true, MOVE_None);

	if (RollSushiMontage)
		PlayAnimMontage(RollSushiMontage);

	GetWorldTimerManager().SetTimer(RollingTimerHandle, this, &ASushiCharacter::FinishRolling, CurrentCookware->RollingTime, false);
}

void ASushiCharacter::FinishRolling()
{
	bIsBusy = false;
	SetIgnoreLookInputAndMovementMode(false, MOVE_Walking);

	if (!HasAuthority()) return;

	if (CurrentCookware)
	{
		CurrentCookware->CookwareState = ECookwareState::RollingDone;
		
		SushiActor = GetWorld()->SpawnActor<AActor>(SushiItemClass, CurrentCookware->GetSushiSpawnLocation()->GetComponentLocation(), FRotator(0, 0, 0));
		SushiActor->SetReplicates(true);
		SushiActor->SetReplicatingMovement(true);
		UserWidget->SetInfoText(FText::FromString(TEXT("Take sushi Roll")));
		
	}

	UE_LOG(LogTemp, Log, TEXT("Rolling finished. Press Interact again to take sushi."));
}

void ASushiCharacter::SetIgnoreLookInputAndMovementMode(const bool bIsIgnoreLookInput, const EMovementMode MovementModeToSet)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->SetIgnoreLookInput(bIsIgnoreLookInput);
		GetCharacterMovement()->SetMovementMode(MovementModeToSet);
	}
}

void ASushiCharacter::TakeSushi()
{
	if (!HasAuthority()) return;
	
	if (!SushiItemClass)
		return;
	
	if (SushiActor)
	{
		SushiActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_R_Socket"));
		HeldSushiName = CurrentCookware->SushiName;
		bSushiInHand = true;
	}
	UserWidget->SetInfoText(FText::FromString(TEXT("Take Sushi to customer's table")));
}

void ASushiCharacter::Interact()
{
	if (bIsBusy)
	{
		UserWidget->SetInfoText(FText::FromString(TEXT("Player is busy, cannot interact now.")));
		return; 
	}

	FVector PlayerLocation = GetActorLocation();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<FOverlapResult> Overlaps;

	bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		PlayerLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_WorldDynamic),
		FCollisionShape::MakeSphere(150.f),
		Params
	);

	DrawDebugSphere(GetWorld(), PlayerLocation, 150.f, 16, FColor::Purple, false, 1.0f);

	if (bHasOverlap)
	{
		ASushiCookware* FoundCookware = nullptr;
		ASushiTable* FoundTable = nullptr;

		for (const FOverlapResult& Result : Overlaps)
		{
			if (!FoundCookware)
			{
				FoundCookware = Cast<ASushiCookware>(Result.GetActor());
				if (FoundCookware)
					continue;
			}

			if (!FoundTable)
			{
				FoundTable = Cast<ASushiTable>(Result.GetActor());
			}
		}

		if (FoundCookware)
		{
			if (bSushiInHand)
			{
				UserWidget->SetInfoText(FText::FromString(TEXT("You already have Sushi in your hands")));
				return; 
			}
			
			CurrentCookware = FoundCookware;
			CurrentCookware->Interact(this);
			return;
		}
		else if (FoundTable)
		{
			FoundTable->TryDeliverSushi(this);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No cookware or table nearby"));
	UserWidget->SetInfoText(FText::FromString(TEXT("Nothing to interact with nearby")));
}

void ASushiCharacter::Drop()
{
	if (!HasAuthority()) return;
	ClearHeldSushi();
}

void ASushiCharacter::ClearHeldSushi()
{
	if (SushiActor)
	{
		SushiActor->Destroy();
		SushiActor = nullptr;
	}
	HeldSushiName = "";
	bSushiInHand = false;

}


void ASushiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASushiCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASushiCharacter::Look);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASushiCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASushiCharacter::Interact);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ASushiCharacter::Drop);
	}
}


void ASushiCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ASushiCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ASushiCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ASushiCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}