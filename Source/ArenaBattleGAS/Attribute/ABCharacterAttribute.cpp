// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABCharacterAttribute.h"
#include <ArenaBattleGAS.h>
#include "GameplayEffectExtension.h"
#include "Tag/ABGameplayTag.h"

UABCharacterAttribute::UABCharacterAttribute() :
	AttackRange(100.0f), MaxAttackRange(300.0f),
	AttackRadius(50.0f), MaxAttackRadius(150.0f),
	AttackRate(30.0f), MaxAttackRate(100.0f),
	MaxHealth(100.0f), Damage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UABCharacterAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

void UABCharacterAttribute::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	/*if (Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}*/
}

bool UABCharacterAttribute::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)			// 참조로 받음.
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(ABTAG_CHARACTER_INVINCIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;		// 리턴값은 크게 상관 없음.
}

void UABCharacterAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(ABTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}
