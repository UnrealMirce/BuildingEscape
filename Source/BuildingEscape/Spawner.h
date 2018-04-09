// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/WorldComposition.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "Spawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API USpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetStaticMeshFromOwner();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnNextActor(FTransform &NextSpawningLocation);

	void ComputeNextSpawnPosition();

private:
	AActor* SpawnerOrigin;
	AActor* DesiredActorToSpawn;
	UStaticMesh *DesiredStaticMesh;

	int32 CurrentXOffset;
	int32 CurrentYOffset;
	int32 CurrentZOffset;
	int32 PassedTicks;
	

		
	
};
