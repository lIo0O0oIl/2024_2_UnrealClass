// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttribute.h"

AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UABCharacterAttribute>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}