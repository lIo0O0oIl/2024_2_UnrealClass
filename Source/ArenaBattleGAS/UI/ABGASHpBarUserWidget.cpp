// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASHpBarUserWidget.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttribute.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>		// UI Control
#include "Tag/ABGameplayTag.h"

void UABGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttribute::GetHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttribute::GetMaxHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnMaxHealthChanged);

		ASC->RegisterGameplayTagEvent(ABTAG_CHARACTER_INVINCIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UABGASHpBarUserWidget::OnInvincibleTagChanged);
		PbHpBar->SetFillColorAndOpacity(HealthColor);

		const UABCharacterAttribute* CurrentAttributeSet = ASC->GetSet<UABCharacterAttribute>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UABGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnInvincibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvincibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

void UABGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, CurrentMaxHealth)));
	}
}
