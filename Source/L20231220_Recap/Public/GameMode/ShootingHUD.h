// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

/**
 *
 */
UCLASS()
class L20231220_RECAP_API AShootingHUD : public AHUD
{
	GENERATED_BODY()



public:
	virtual void BeginPlay();

public:
	void BindMyPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyHp(float CurHp, float MaxHp);

	void OnUpdateMyHp_Implementation(float CurHp, float MaxHp);

public:
	//TSubclassOf : 지정한 클래스와 클래스를 상속한 모든 클래스를 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HudWidget;
};
