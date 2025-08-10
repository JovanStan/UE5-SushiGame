
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SushiCharacter.generated.h"


class USushiUserWidget;
class ASushiCookware;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class ASushiCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent>FollowCamera;
	
	bool bIsBusy = false;
	UPROPERTY()
	TObjectPtr<ASushiCookware> CurrentCookware;

	FTimerHandle RollingTimerHandle;
	FTimerHandle CuttingTimerHandle;


	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MouseLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> DropAction;

public:
	ASushiCharacter();
	
	void Interact();
	void Drop();
	
	void StartCutting(ASushiCookware* Cookware);
	void StartRolling(ASushiCookware* Cookware);
	void TakeSushi();
	
	void FinishCutting();
	void FinishRolling();

	void ClearHeldSushi();

	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);
	void SetIgnoreLookInputAndMovementMode(bool bIsIgnoreLookInput, EMovementMode MovementModeToSet);

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, Category="Sushi|Animations")
	TObjectPtr<UAnimMontage> CutFishMontage;

	UPROPERTY(EditAnywhere, Category="Sushi|Animations")
	TObjectPtr<UAnimMontage> RollSushiMontage;

	UPROPERTY(EditAnywhere, Category="Sushi|Items")
	TSubclassOf<AActor> SushiItemClass;
	UPROPERTY()
	TObjectPtr<AActor> SushiActor;
	bool bSushiInHand;
	FName HeldSushiName;
	

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere)
	TSubclassOf<USushiUserWidget> UserWidgetClass;
	UPROPERTY()
	TObjectPtr<USushiUserWidget> UserWidget;

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE  UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE FName GetHeldSushiName() const { return HeldSushiName; }
	FORCEINLINE AActor* GetSushiActor() const { return SushiActor; }
};

