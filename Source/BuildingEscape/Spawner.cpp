// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"


// Sets default values for this component's properties
USpawner::USpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnerOrigin = this->GetOwner();
	GetStaticMeshFromOwner();
}

// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ComputeNextSpawnPosition();
	float delta = 10.0f;
	FVector Scaling(0.1f, 0.1f, 0.1f);
	FVector NextSpawningLocation = SpawnerOrigin->GetActorLocation() + FVector(CurrentXOffset * delta, CurrentYOffset * delta, CurrentZOffset * delta);
	FTransform NextSpawnTransform = FTransform(SpawnerOrigin->GetActorRotation(), NextSpawningLocation, Scaling);
	SpawnNextActor(NextSpawnTransform);
}

// Auxiliary

void USpawner::GetStaticMeshFromOwner()
{
	TArray<UStaticMeshComponent*> Components;
	SpawnerOrigin->GetComponents<UStaticMeshComponent>(Components);
	if (Components.Num() > 0) {
		DesiredStaticMesh = Components[0]->GetStaticMesh();
	}
}

void USpawner::SpawnNextActor(FTransform &NextSpawningLocation)
{
	UClass* param = SpawnerOrigin->GetClass();
	AActor* spawned = GetWorld()->SpawnActor(param, &NextSpawningLocation, FActorSpawnParameters());
	FVector f = spawned->GetActorLocation();

	TArray<UStaticMeshComponent*> Components;
	spawned->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++) {
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		StaticMeshComponent->SetMobility(EComponentMobility::Movable);
		StaticMeshComponent->SetStaticMesh(DesiredStaticMesh);
	}
}

void USpawner::ComputeNextSpawnPosition()
{
	PassedTicks++;
	CurrentXOffset++;
	if (PassedTicks % 100 == 0) {
		CurrentXOffset = 0;
		CurrentYOffset++;
	}
	if (PassedTicks % 10000 == 0) {
		CurrentXOffset = 0;
		CurrentYOffset = 0;
		CurrentZOffset++;
	}
}

