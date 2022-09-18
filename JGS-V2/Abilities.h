#pragma once

#include "framework.h"

inline auto InternalTryActivateAbility = reinterpret_cast<bool(*)(UAbilitySystemComponent* Comp, FGameplayAbilitySpecHandle Handle, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, FGameplayEventData* TriggerEventData)>(GetBaseAddress() + 0x2C7D040);
inline auto GiveAbility = reinterpret_cast<FGameplayAbilitySpecHandle(*)(UAbilitySystemComponent * Comp, FGameplayAbilitySpecHandle * outHandle, FGameplayAbilitySpec)>(GetBaseAddress() + 0x2C7BDC0);

class Abilities
{
public:
	static void InternalServerTryActiveAbility(UAbilitySystemComponent* Comp, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData);
	static void ServerAbilityRPCBatch_Internal(UAbilitySystemComponent* Comp, FGameplayAbilities_FServerAbilityRPCBatch& BatchInfo);
	static void GrantAbility(UAbilitySystemComponent* Comp, UClass* AbilityClass);
	static void GrantAbilties(UAbilitySystemComponent* Comp);
};