// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/ShootingPlayerState.h"


void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass);
	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();


	BindMyPlayerState();
}

void AShootingHUD::BindMyPlayerState()
{
	FTimerHandle th_BindMyPlayerState;
	APlayerController* pc = GetWorld()->GetFirstPlayerController();//player 0
	if (IsValid(pc))
	{
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);
		if (IsValid(ps))
		{
			ps->m_Dele_UpdateHp.AddDynamic(this, &AShootingHUD::OnUpdateMyHp);
			OnUpdateMyHp(ps->m_CurHp, 100);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("AShootingHUD Bind Success!"));
			return;
		}
		
	}
	
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(th_BindMyPlayerState, this, &AShootingHUD::BindMyPlayerState, 0.01f, false);
}

void AShootingHUD::OnUpdateMyHp_Implementation(float CurHp, float MaxHp)
{
}
