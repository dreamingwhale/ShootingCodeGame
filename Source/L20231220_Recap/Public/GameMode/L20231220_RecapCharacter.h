// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "L20231220_RecapCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AL20231220_RecapCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** PressF Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressFAction;

	/** Reload Input Action*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

public:
	AL20231220_RecapCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for Shooting input */
	void Shoot(const FInputActionValue& Value);

	/** Called for PressF input */
	void PressF(const FInputActionValue& Value);
			
	/** Called for Reload input */
	void Reload(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DletaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	//匙飘况农 内靛康开
	//Run On Server
	UFUNCTION(Server, Reliable)
	void ReqPressF();

	//Multicast
	UFUNCTION(NetMulticast,Reliable)
	void ResPressF();
	
	//Multicast
	UFUNCTION(Client,Reliable)
	void ResPressFClient();

	UFUNCTION(Server, Reliable)
	void ReqShoot();

	UFUNCTION(NetMulticast,Reliable)
	void ResShoot();

	UFUNCTION(Server,Reliable)
	void ReqReload();

	UFUNCTION(NetMulticast, Reliable)
	void ResReload();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;

	UPROPERTY(Replicated,BlueprintReadWrite)
	FRotator ControlRot;
};

