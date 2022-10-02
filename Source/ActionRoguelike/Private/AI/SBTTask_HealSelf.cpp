// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "AI/SAICharacter.h"
#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ASAICharacter* MyPawn = Cast<ASAICharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		if (!USAttributeComponent::IsActorAlive(MyPawn))
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* MyAttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

		return MyAttributeComp->ApplyHealthChange(nullptr, MyAttributeComp->GetHealthMax()) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	else return EBTNodeResult::Failed;
}
