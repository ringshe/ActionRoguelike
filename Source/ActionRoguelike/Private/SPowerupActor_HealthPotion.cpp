// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerupActor_HealthPotion::ASPowerupActor_HealthPotion()
{
	CreditCost = 50;
}

void ASPowerupActor_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) return;

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(nullptr, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
		
	}
}
