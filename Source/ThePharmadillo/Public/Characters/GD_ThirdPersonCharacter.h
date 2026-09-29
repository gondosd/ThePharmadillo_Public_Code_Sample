// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterComponents/GD_InteractionHandlerComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GD_ThirdPersonCharacter.generated.h"

class UGD_CharacterMovementComponent;
class UFrontendGameUserSettings;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChanged, float, Energy);

UCLASS(config=Game)
class AGD_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=CameraSettings)
	float MovementFollowingCameraTurnRate = 2.0f;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
	TObjectPtr<UGD_CharacterMovementComponent> ArmadilloCharacterMovementComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGD_InteractionHandlerComponent> InteractionHandler;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

public:
	AGD_ThirdPersonCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal,
	                                              const FVector& PreviousLocation, float TimeDelta) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable)
	void SetInputEnabling(bool bIsEnabled);
	
	//Will Teleport the player to the last valid position, if there is no navmesh, to the Spawn Point
	UFUNCTION(BlueprintCallable)
	void ResetCharactersTransform();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_JumpPreparation();
	virtual bool CanJumpInternal_Implementation() const override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	//Handles looking at the moving direction if setting dictates so
	void AutoLook(float DeltaSeconds) const;
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//HandlesEnergyDraingin if current movement dictates so
	void DrainEnergy(float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable)
	void SetEnergy(const float NewEnergy);
	
	UFUNCTION(BlueprintCallable)
	void GrantLimitlessEnergy ();
	
	void FallAsleep();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "FallAsleep", Category = Energy)
	void BP_FallAsleep();

private:
	
	void CheckForValidGround();
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UGD_CharacterMovementComponent* GetGDCharacterMovement() const { return ArmadilloCharacterMovementComponent; }

	FCollisionQueryParams GetIgnoreCharacterParams() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UFrontendGameUserSettings> CachedUserSettings;
	UPROPERTY(Transient)
	TObjectPtr<APlayerController> PossessingPlayerController;

	// FQuat MeshRotationQuat = FQuat::Identity;
	// float RollDecayMultiplier = 1.f;
	// float RollDecaySpeed = 0.02f;
	
	FTimerHandle CoyoteTimeHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=CoyoteTime, meta=(AllowPrivateAccess="true"))
	float CoyoteTimeDuration = 0.1f;
	
	FTimerHandle GroundCheckerTimerHandle;
	UPROPERTY(SaveGame)
	FTransform LastValidTransform;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category=Energy, meta=(AllowPrivateAccess="true"))
	FOnEnergyChanged OnEnergyChanged;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Energy, meta=(AllowPrivateAccess="true"))
	float EnergyDrainSpeed = 0.001f;
	
	UPROPERTY(SaveGame)
	float Energy = 100.0f;
	
	UPROPERTY(Transient)
	bool bLimitlessEnergy = false;
};
