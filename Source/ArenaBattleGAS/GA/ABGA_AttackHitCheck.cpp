// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_AttackHitCheck.h"
#include "ArenaBattleGAS.h"
#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"		
#include <AbilitySystemBlueprintLibrary.h>
#include "Attribute/ABCharacterAttribute.h"
#include "Tag/ABGameplayTag.h"

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABGAS_LOG(LogABGAS, Log, TEXT("%s"), *TriggerEventData->EventTag.GetTagName().ToString());

	CurrentLevel = TriggerEventData->EventMagnitude;

	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, AABTA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Dected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		if (!SourceASC || !TargetASC)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
			return;
		}

		const UABCharacterAttribute* SourceAttribute = SourceASC->GetSet<UABCharacterAttribute>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			//EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
			TargetASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT,
				CueParam);
		}

		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
		if (BuffEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}