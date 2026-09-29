// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/GD_ThirdPersonCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NavigationSystem.h"
#include "Characters/CharacterComponents/GD_CharacterMovementComponent.h"
#include "Characters/CharacterComponents/GD_InteractionHandlerComponent.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGD_ThirdPersonCharacter

AGD_ThirdPersonCharacter::AGD_ThirdPersonCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGD_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	ArmadilloCharacterMovementComponent = Cast<UGD_CharacterMovementComponent>(GetCharacterMovement());

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create interactionhandler component
	InteractionHandler = CreateDefaultSubobject<UGD_InteractionHandlerComponent>(TEXT("InteractionHandler"));
	InteractionHandler->SetupAttachment(RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGD_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	CachedUserSettings = Cast<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());

	LastValidTransform = GetActorTransform(); //if we don't step on navmesh, the spawning location is the fallback

	//Every second it checks if we walk on a valid navmesh to set LastValidTransform
	GetWorld()->GetTimerManager().SetTimer(GroundCheckerTimerHandle, this, &ThisClass::CheckForValidGround, 1.f, true);
}

void AGD_ThirdPersonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PossessingPlayerController = Cast<APlayerController>(GetController());
}

void AGD_ThirdPersonCharacter::UnPossessed()
{
	Super::UnPossessed();
	PossessingPlayerController = nullptr;
}

void AGD_ThirdPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AutoLook(DeltaSeconds);
	DrainEnergy(DeltaSeconds);
}

void AGD_ThirdPersonCharacter::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal,
                                                                const FVector& PreviousLocation, float TimeDelta)
{
	GetWorldTimerManager().SetTimer(CoyoteTimeHandle, [this]() { CoyoteTimeHandle.Invalidate(); }, CoyoteTimeDuration, false);
}

void AGD_ThirdPersonCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(GroundCheckerTimerHandle);
	GetWorldTimerManager().ClearTimer(CoyoteTimeHandle);
}

void AGD_ThirdPersonCharacter::SetInputEnabling(const bool bIsEnabled)
{
	if (bIsEnabled)
		EnableInput(PossessingPlayerController);
	else
		DisableInput(PossessingPlayerController);
}

void AGD_ThirdPersonCharacter::ResetCharactersTransform()
{
	SetActorTransform(LastValidTransform);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGD_ThirdPersonCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGD_ThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Ongoing, this, &ThisClass::BP_JumpPreparation);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGD_ThirdPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGD_ThirdPersonCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AGD_ThirdPersonCharacter::DrainEnergy(float DeltaSeconds)
{
	if (Energy <= 0.f) return; // if we are out of energy there is no need to drain further
	if (ArmadilloCharacterMovementComponent->IsFalling()) return; //we arent getting tired from falling

	const FVector Velocity2D = GetVelocity() * FVector(1.0f, 1.0f, 0.0f);
	const float Speed2D = Velocity2D.Length();

	SetEnergy(Energy - (Speed2D * EnergyDrainSpeed * DeltaSeconds));
}

void AGD_ThirdPersonCharacter::SetEnergy(const float NewEnergy)
{
	if (bLimitlessEnergy) return;

	Energy = NewEnergy;
	OnEnergyChanged.Broadcast(Energy);

	if (Energy <= 0.f)
		FallAsleep();
}

void AGD_ThirdPersonCharacter::GrantLimitlessEnergy()
{
	bLimitlessEnergy = true;
	SetEnergy(100.f);
}

void AGD_ThirdPersonCharacter::FallAsleep()
{
	//TODO: play anim of falling asleep + Scarab friend dragging anim
	GetGDCharacterMovement()->ForceCrouch();

	DisableInput(PossessingPlayerController);
	BP_FallAsleep();
}

void AGD_ThirdPersonCharacter::CheckForValidGround()
{
	{ 
		if (!ArmadilloCharacterMovementComponent->IsMovingOnGround()) return; // not moving on the ground -> not valid Transform

		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSys) return; // No active navmesh -> not valid Transform

		FNavLocation OutLocation;
		const FVector QueryExtent(20.f, 20.f, 200.f);
		if (!NavSys->ProjectPointToNavigation(GetActorLocation(), OutLocation, QueryExtent)) return; // No active navmesh location -> not valid Transform
	}

	//If every check passed, we overwrite the fallback value
	LastValidTransform = GetActorTransform();
}


bool AGD_ThirdPersonCharacter::CanJumpInternal_Implementation() const
{
	return CoyoteTimeHandle.IsValid() || JumpIsAllowedInternal();
}

void AGD_ThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGD_ThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	float InvertValue = CachedUserSettings->GetInvertLook() ? -1.f : 1.f;
	float Sensitivity = CachedUserSettings->GetLookSensitivity();
	FVector2D LookAxisVector = Value.Get<FVector2D>() * Sensitivity * FVector2D(InvertValue, 1.f);

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGD_ThirdPersonCharacter::AutoLook(float DeltaSeconds) const
{
	if (const FVector InputVector = GetCharacterMovement()->GetCurrentAcceleration();
		!InputVector.IsNearlyZero() &&
		IsCrouched() && //Only look towards your roll direction
		CachedUserSettings->GetAllowCameraFollowRollMovement())
	{
		FRotator ControlRot = GetControlRotation();
		const FRotator ActorRot = GetActorRotation();

		const FRotator TargetRot = FMath::RInterpTo(ControlRot, ActorRot, DeltaSeconds, MovementFollowingCameraTurnRate);
		ControlRot.Yaw = TargetRot.Yaw;
		GetController()->SetControlRotation(ControlRot);
	}
}


FCollisionQueryParams AGD_ThirdPersonCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}
