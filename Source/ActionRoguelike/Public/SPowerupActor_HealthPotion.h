// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "Components/MeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SPowerupActor_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	int32 CreditCost;
public:
	ASPowerupActor_HealthPotion();

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
