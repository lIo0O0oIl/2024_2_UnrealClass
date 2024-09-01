// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UABAT_JumpAndWaitForLanding();

public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);

public:
	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;
};
