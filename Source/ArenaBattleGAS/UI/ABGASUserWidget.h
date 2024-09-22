// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitysystemInterface.h"
#include "ABGASUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void SetAbilitySystemComponent(AActor* InOwner);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;
};
