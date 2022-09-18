#include "Abilities.h"
#include "Util.h"

FGameplayAbilitySpec* FindAbilitySpecFromHandle(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle Handle)
{
	for (int i = 0; i < AbilitySystem->ActivatableAbilities.Items.Num(); i++)
	{
		auto Spec = AbilitySystem->ActivatableAbilities.Items[i];

		if (Spec.Handle.Handle == Handle.Handle)
		{
			return &Spec;
		}
	}

	return nullptr;
}

void Abilities::InternalServerTryActiveAbility(UAbilitySystemComponent* Comp, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Comp, Handle);
	if (!Spec)
	{
		LOG("InternalServerTryActiveAbility. Rejecting ClientActivation of ability with invalid SpecHandle!");
		Comp->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	//ConsumeAllReplicatedData(Handle, PredictionKey);

	UGameplayAbility* AbilityToActivate = Spec->Ability;

	UGameplayAbility* InstancedAbility = nullptr;
	Spec->InputPressed = true;

	if (InternalTryActivateAbility(Comp, Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
	{

	} else {
		LOG("InternalServerTryActiveAbility. Rejecting ClientActivation of {}. InternalTryActivateAbility failed!", AbilityToActivate->GetName());
		Comp->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		Spec->InputPressed = false;
	}
}

void Abilities::ServerAbilityRPCBatch_Internal(UAbilitySystemComponent* Comp, FGameplayAbilities_FServerAbilityRPCBatch& BatchInfo)
{
	Comp->ServerTryActivateAbility(BatchInfo.AbilitySpecHandle, BatchInfo.InputPressed, BatchInfo.PredictionKey);
	Comp->ServerSetReplicatedTargetData(BatchInfo.AbilitySpecHandle, BatchInfo.PredictionKey, BatchInfo.TargetData, FGameplayTag(), BatchInfo.PredictionKey);

	if (BatchInfo.Ended)
	{
		FGameplayAbilityActivationInfo FakeInfo = FGameplayAbilityActivationInfo();
		FakeInfo.PredictionKeyWhenActivated = BatchInfo.PredictionKey;
		Comp->ServerEndAbility(BatchInfo.AbilitySpecHandle, FakeInfo, BatchInfo.PredictionKey);
	}
}

void Abilities::GrantAbility(UAbilitySystemComponent* Comp, UClass* AbilityClass)
{
	FGameplayAbilitySpecHandle Handle = *new FGameplayAbilitySpecHandle();
	Handle.Handle = rand();

	FGameplayAbilitySpec Spec = *new FGameplayAbilitySpec{ -1, -1, -1, Handle, AbilityClass->CreateDefaultObject<UGameplayAbility>(), 1, -1, nullptr, 0, false, false, false };

	for (int i = 0; i < Comp->ActivatableAbilities.Items.Num(); i++)
	{
		auto& CurrentSpec = Comp->ActivatableAbilities.Items[i];

		if (CurrentSpec.Ability == Spec.Ability)
			return;
	}

	GiveAbility(Comp, &Spec.Handle, Spec);
}

void Abilities::GrantAbilties(UAbilitySystemComponent* Comp)
{
	static auto GAS = FindObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");

	for (int i = 0; i < GAS->GameplayAbilities.Num(); i++)
	{
		auto Ability = GAS->GameplayAbilities[i];

		GrantAbility(Comp, Ability);
	}
}
