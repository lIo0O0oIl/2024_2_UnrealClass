// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASUserWidget.h"
#include <AbilitySystemBlueprintLibrary.h>

UAbilitySystemComponent* UABGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}

void UABGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}