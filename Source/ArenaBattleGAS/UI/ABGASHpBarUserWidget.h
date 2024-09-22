// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "ABGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASHpBarUserWidget : public UABGASUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvincibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHpBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;		// 나누는 애는 0.0 으로 초기화 잘 안함.

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvincibleColor = FLinearColor::Blue;

};
