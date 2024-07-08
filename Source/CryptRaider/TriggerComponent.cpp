// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // this allows the component to tick; by default components don't tick

	// ...
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!mover)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mover is not set!"));
		return;
	}

	AActor* actor = GetAcceptableActor();
	if (actor) {
		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		if (component)
		{
			component->SetSimulatePhysics(false);
		}
		actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform); // automaticaly attach the actor to a component
		mover->SetShouldMove(true);
	}
	else
	{
		mover->SetShouldMove(false);
	}
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> actors;
	GetOverlappingActors(actors);

	for (AActor* actor : actors)
	{
		if (actor->ActorHasTag(acceptableActorTag) && !actor->ActorHasTag("Grabbed"))
		{
			return actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* newMover)
{
	mover = newMover;
}

