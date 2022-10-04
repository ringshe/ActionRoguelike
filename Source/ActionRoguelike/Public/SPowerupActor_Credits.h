// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerupActor_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor_Credits : public ASPowerupActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsAmount;

public:
	ASPowerupActor_Credits();

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
