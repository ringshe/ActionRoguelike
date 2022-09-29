// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/SAICharacter.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	//SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	//SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	DamageAmount = 50.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DrawDebugSphere(GetWorld(), SweepResult.ImpactPoint, 30.0f, 16, FColor::Red, false, 5.0f);
	UE_LOG(LogTemp, Log, TEXT("%i"), OtherActor->GetClass() == GetInstigator()->GetClass());
	if (OtherActor && OtherActor != GetInstigator() && !(OtherActor->IsA<ASAICharacter>() && GetInstigator()->IsA<ASAICharacter>()))
	{
		
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);
			Destroy();
		}
	}
}

