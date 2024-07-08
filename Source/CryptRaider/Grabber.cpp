// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"

#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if (physicsHandle->GetGrabbedComponent())
	{
		FVector targetLocation = GetComponentLocation() + GetForwardVector() * holdDistance;
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& hitResult) const
{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * maxGrabDistance;
	//DrawDebugLine(GetWorld(), start, end, FColor::Red);
	//DrawDebugSphere(GetWorld(), end, 10, 10, FColor::Blue, false, 5);

	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	return GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, sphere);
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	FHitResult hitResult;
	if (GetGrabbableInReach(hitResult))
	{
		// AActor* actor = hitResult.GetActor();
		// UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *actor->GetActorNameOrLabel());
		// FVector hitLocation = hitResult.Location; // the place of the **sphere** that a hit could happen
		// FVector hitPoint = hitResult.ImpactPoint; // the place of the actual hit point
		// DrawDebugSphere(GetWorld(), hitLocation, 10, 10, FColor::Blue, false, 5);
		// DrawDebugSphere(GetWorld(), hitPoint, 10, 10, FColor::Red, false, 5);

		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->WakeAllRigidBodies();
		hitComponent->SetSimulatePhysics(true);
		AActor* hitActor = hitResult.GetActor();
		hitComponent->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		hitActor->Tags.Add("Grabbed");
		// hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		physicsHandle->GrabComponentAtLocationWithRotation(hitComponent, NAME_None, hitResult.ImpactPoint, GetComponentRotation()); // grab the hit component
	}
}

void UGrabber::Release() 
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if (!physicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics handle!"));
		return;
	}
	UPrimitiveComponent* component = physicsHandle->GetGrabbedComponent();
	if (component)
	{
		component->WakeAllRigidBodies();
		component->GetOwner()->Tags.Remove("Grabbed");
		physicsHandle->ReleaseComponent();
	}
	UE_LOG(LogTemp, Display, TEXT("Grabber released!"));
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!result)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent"));
	}
	return result;
}

