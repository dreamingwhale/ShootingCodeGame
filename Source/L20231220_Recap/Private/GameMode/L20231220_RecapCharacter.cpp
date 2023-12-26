// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/L20231220_RecapCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Net/UnrealNetwork.h>
#include "GameMode/ShootingPlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AL20231220_RecapCharacter

AL20231220_RecapCharacter::AL20231220_RecapCharacter()
{
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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AL20231220_RecapCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AL20231220_RecapCharacter, ControlRot);
}

void AL20231220_RecapCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AL20231220_RecapCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() == true)
	{
		ControlRot = GetControlRotation();
	}

}

void AL20231220_RecapCharacter::ReqShoot_Implementation()
{
	ResShoot();
}

void AL20231220_RecapCharacter::ResShoot_Implementation()
{
	PlayAnimMontage(ShootMontage);
}

void AL20231220_RecapCharacter::ReqPressF_Implementation()
{
	ResPressF();
}

void AL20231220_RecapCharacter::ResPressF_Implementation()
{
	AShootingPlayerState* ps = Cast<AShootingPlayerState>(GetPlayerState());
	if (IsValid(ps) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("PS is not Valid"));
		return;
	}
	ps->AddDamage(10.0f);
}

void AL20231220_RecapCharacter::ResPressFClient_Implementation()
{
}

void AL20231220_RecapCharacter::ReqReload_Implementation()
{
	ResReload();
}

void AL20231220_RecapCharacter::ResReload_Implementation()
{
	PlayAnimMontage(ReloadMontage);
}


//////////////////////////////////////////////////////////////////////////
// Input

void AL20231220_RecapCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AL20231220_RecapCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AL20231220_RecapCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AL20231220_RecapCharacter::Shoot);

		// PressF
		EnhancedInputComponent->BindAction(PressFAction, ETriggerEvent::Started, this, &AL20231220_RecapCharacter::PressF);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AL20231220_RecapCharacter::Reload);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AL20231220_RecapCharacter::Move(const FInputActionValue& Value)
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

void AL20231220_RecapCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AL20231220_RecapCharacter::Shoot(const FInputActionValue& Value)
{
	ReqShoot();

}

void AL20231220_RecapCharacter::PressF(const FInputActionValue& Value)
{
	ReqPressF();
}

void AL20231220_RecapCharacter::Reload(const FInputActionValue& Value)
{
	ReqReload();
}

