// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ABGC_AttackHIt.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGC_AttackHIt : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UABGC_AttackHIt();			// Hit......

public:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ParticleSystem;
};
