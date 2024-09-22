// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttribute.h"
#include "UI/ABGASUserWidget.h"
#include "UI/ABGASWidgetComponent.h"

AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UABCharacterAttribute>(TEXT("AttributeSet"));

	HpBar = CreateDefaultSubobject<UABGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);			// 빌보드 타입의 UI
		HpBar->SetDrawSize(FVector2D(200.0f, 20.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);		// ThisClass 말 그대로 나

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);

	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AABGASCharacterNonPlayer::OnOutOfHealth()
{
	SetDead();
}
