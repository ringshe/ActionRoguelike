// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
    ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);
    TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
		//DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
	}
}


void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        if (ActiveHealthBar == nullptr)
        {
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
                ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
        }
        

        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
        if (NewHealth <= 0.0f)
        {
            // stop BT 
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }
            // ragdoll 
            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");
            
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
            GetCharacterMovement()->DisableMovement();
            // set lifespan
            SetLifeSpan(10.0f);
        }
    }
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
    SetTargetActor(Pawn);
}

