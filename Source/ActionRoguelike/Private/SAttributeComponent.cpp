// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
}



bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("ApplyHealthChange %f"), Delta);
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

