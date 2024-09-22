// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ABCharacterAttribute.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABCharacterAttribute : public UAttributeSet		// 선생님꺼는 Set 이 뒤에 붙어있음.. 잘못만듦.
{
	GENERATED_BODY()

protected:
	friend class UABGE_AttackDamage;

public:
	UABCharacterAttribute();
	
public:
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, AttackRange);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, AttackRadius);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, AttackRate);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, Health);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, MaxHealth);
	ATTRIBUTE_ACCESSORS(UABCharacterAttribute, Damage);

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	mutable FOutOfHealthDelegate OnOutOfHealth;		// const 함수로도 이 값을 변화시킬 수 있음.

protected:
	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))		// 메타는 언리얼에서 이용 가능하도록 하는거
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	bool bOutOfHealth = false;
};
