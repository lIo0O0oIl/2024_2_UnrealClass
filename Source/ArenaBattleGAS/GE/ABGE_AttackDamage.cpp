// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABGE_AttackDamage.h"
#include "Attribute/ABCharacterAttribute.h"

UABGE_AttackDamage::UABGE_AttackDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UABCharacterAttribute::StaticClass(), GET_MEMBER_NAME_CHECKED(UABCharacterAttribute, Health)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat DamageAmount(-30.f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);

	HealthModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(HealthModifier);
}
