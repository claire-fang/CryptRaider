// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();
	originalLocation = GetOwner()->GetActorLocation(); // get the original location of the actor
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector current = GetOwner()->GetActorLocation();
	FVector target = originalLocation;
	if (shouldMove)
	{
		target = originalLocation + moveOffset;
	}

	float speed = moveOffset.Length() / moveTime; // calculate the move speed
	FVector newLocation = FMath::VInterpConstantTo(current, target, DeltaTime, speed);
	GetOwner()->SetActorLocation(newLocation);
}

void UMover::SetShouldMove(bool newShouldMove)
{
	shouldMove = newShouldMove;
}

