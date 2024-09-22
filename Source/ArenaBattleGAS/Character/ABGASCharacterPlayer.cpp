// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/ABGASPlayerState.h"
#include <EnhancedInputComponent.h>
#include "UI/ABGASUserWidget.h"
#include "UI/ABGASWidgetComponent.h"
#include <Attribute/ABCharacterAttribute.h>
#include "Tag/ABGameplayTag.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	ASC = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackWyrmBlade/SK_Blade_BlackWyrmBlade.SK_Blade_BlackWyrmBlade"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh = WeaponMeshRef.Object;
	}

	WeaponRange = 75.0f;
	WeaponAttackRate = 100.0f;
}

UAbilitySystemComponent* AABGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AABGASPlayerState* GASPS = GetPlayerState<AABGASPlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AABGASCharacterPlayer::EquipWeapon);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AABGASCharacterPlayer::UnequipWeapon);

		const UABCharacterAttribute* CurrentAttributeSet = ASC->GetSet<UABCharacterAttribute>();
		if (CurrentAttributeSet)
		{
			CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);		// ThisClass 말 그대로 나
		}

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);		// 발동시킬 GA 등록
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		APlayerController* PlayerContorller = CastChecked<APlayerController>(NewController);
		PlayerContorller->ConsoleCommand(TEXT("showdebug abilitysytem"));
	}
}

void AABGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsValid(ASC) && IsValid(PlayerInputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABGASCharacterPlayer::GASInputReleased, 0);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 1);
	}
}

void AABGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AABGASCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}

void AABGASCharacterPlayer::EquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(WeaponMesh);

		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABCharacterAttribute::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABCharacterAttribute::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UABCharacterAttribute::GetAttackRangeAttribute(), CurrentAttackRange + WeaponRange);
		ASC->SetNumericAttributeBase(UABCharacterAttribute::GetAttackRateAttribute(), CurrentAttackRate + WeaponAttackRate);
	}
}

void AABGASCharacterPlayer::UnequipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(nullptr);

		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABCharacterAttribute::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABCharacterAttribute::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UABCharacterAttribute::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		ASC->SetNumericAttributeBase(UABCharacterAttribute::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);
	}
}
