// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_JumpAndWaitForLanding.h"
#include <GameFramework/Character.h>

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);

	Character->Jump();

	SetWaitingOnAvatar();
}

void UABAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
}

void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())		// 알려줄지 말지 정할 수 있음
	{
		OnComplete.Broadcast();		// 멀티여서 방송처럼 호출함.
	}
}
