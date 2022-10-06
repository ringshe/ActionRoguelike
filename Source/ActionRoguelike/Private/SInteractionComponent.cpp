
// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "GameFramework/CharacterMovementComponent.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceRadius = 10.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	FVector Start;
	FVector End;

	ASCharacter* MyCharacter = Cast<ASCharacter>(MyOwner);
	if (MyCharacter)
	{
		UCameraComponent* CameraComp = Cast<UCameraComponent>(MyCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
		FVector CameraLocation = CameraComp->GetComponentLocation();
		FRotator CameraRotator = CameraComp->GetComponentRotation();
		FVector TraceStart = CameraLocation;
		FVector TraceEnd = CameraLocation + (CameraRotator.Vector() * TraceDistance);
		FHitResult Hit;
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(CollisionChannel);
		FCollisionShape Shape;
		Shape.SetSphere(TraceRadius);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MyOwner);
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			Start = MyCharacter->GetMesh()->GetSocketLocation("Eyes_Position");
			End = Hit.ImpactPoint;
		}
		else 
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			Start = EyeLocation;
			End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
		}
	}
	else
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		Start = EyeLocation;

		End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	}
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;

	for (FHitResult& Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, LineColor, false, 2.0f);
		}
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
		
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
	//FRotator NewRotator = FRotationMatrix::MakeFromX(End - Start).Rotator();
	//NewRotator.Pitch = 0.f;
	//NewRotator.Roll = 0.f;
	//ASCharacter* MyCharacter = Cast<ASCharacter>(MyOwner);
	////MyCharacter->AddMovementInput(NewRotator.Vector(), 0.01);
	//MyCharacter->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, NewRotator, false);
	//
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No FocusActor to interact.");
		return;
	}
	AActor* MyOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyOwner);
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

