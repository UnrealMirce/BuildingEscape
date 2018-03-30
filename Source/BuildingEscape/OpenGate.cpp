// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenGate.h"


// Sets default values for this component's properties
UOpenGate::UOpenGate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenGate::BeginPlay()
{
	Super::BeginPlay();

	lastOpenTime = 0;
	startingPosition = GetOwner()->GetActorLocation();
	playerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenGate::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float movementVector = 1.0f;
	if (pressurePad->IsOverlappingActor(playerActor)) {
		lastOpenTime = GetWorld()->GetTimeSeconds();
		OpenDoor();
	}

	if (GetWorld()->GetTimeSeconds() - lastOpenTime > 1.0f) {
		CloseDoor();
	}

	// ...
}

void UOpenGate::OpenDoor() {
	FVector nextLocation = FVector(startingPosition.X, startingPosition.Y, startingPosition.Z + 500);
	GetOwner()->SetActorLocation(nextLocation);
}

void UOpenGate::CloseDoor() {
	FVector nextLocation = FVector(startingPosition.X, startingPosition.Y, startingPosition.Z);
	GetOwner()->SetActorLocation(nextLocation);
}

