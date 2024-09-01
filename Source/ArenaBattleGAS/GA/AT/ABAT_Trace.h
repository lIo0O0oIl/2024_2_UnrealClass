// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UABAT_Trace();
	
public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AABTA_Trace> TargetActorClass);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AABTA_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AABTA_Trace> SpawnedTargetActor;
};
