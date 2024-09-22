// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/ABGC_AttackHIt.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UABGC_AttackHIt::UABGC_AttackHIt()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Game/ParticleContent/Particles/P_Explosion.P_Explosion"));
	if (ExplosionRef.Object)
	{
		ParticleSystem = ExplosionRef.Object;
	}
}

bool UABGC_AttackHIt::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	return false;
}
