// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UINTERFACE(Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class L20231220_RECAP_API IWeaponInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void EventTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();
};
