// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include <GameFramework/RotatingMovementComponent.h>
#include <AbilitySystemComponent.h>
#include <GameplayAbilitySpec.h>
#include "ArenaBattleGAS.h"		// 로그 사용할 수 있도록
#include <Tag/ABGameplayTag.h>
#include <Abilities/GameplayAbility.h>

AABGASFountain::AABGASFountain()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();		// 오버라이딩 한 순간 권한을 뺐어서 기존 프레임워크에 있던거 호출해줄려고

	RotatingMovement->bAutoActivate = false;		// 지금은 움직이짐마
	RotatingMovement->Deactivate();		// 회전도 멈춰놔

	ASC->InitAbilityActorInfo(this, this);		// 오너가 플레이어, 아바타는 플레이어가 움직이는 애
	// 액터에게 붙이는 시스템인데 데이터랑 실제로 보이는 부분이랑이 있어야함.

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpce(StartAbility);
		ASC->GiveAbility(StartSpce);		// 발동시킬 GA 등록
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	//FGameplayAbilitySpec* RotateGASSpec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());
	//if (!RotateGASSpec)
	//{
	//	ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
	//	return;
	//}

	//if (!RotateGASSpec->IsActive())
	//{
	//	// 핸들을 지정하여 발동
	//	ASC->TryActivateAbility(RotateGASSpec->Handle);
	//}
	//else
	//{
	//	// 비활성화하여 취소
	//	ASC->CancelAbilityHandle(RotateGASSpec->Handle);
	//}

	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);
	if (!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}
}
