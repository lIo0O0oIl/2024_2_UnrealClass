// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ABGASItemBox.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASItemBox : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AABGASItemBox();

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

protected:
	virtual void PostInitializeComponents() override;

	void InvokeGameplayCue(AActor* Target);
	void ApplyEffectToTarget(AActor* Target);

protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Gategories = GameplayCue))		// 영역 제한
	FGameplayTag GameplayCueTag;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;
};
