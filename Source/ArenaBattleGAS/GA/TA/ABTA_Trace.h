// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ABTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AABTA_Trace();

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

public:
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

protected:
	bool bShowDebug = false;
};
