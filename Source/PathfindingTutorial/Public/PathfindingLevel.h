// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PathfindingTerrain.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindingLevel.generated.h"

UCLASS()
class PATHFINDINGTUTORIAL_API APathfindingLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindingLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APathfindingTerrain>> possible_tiles;

	UPROPERTY(EditAnywhere)
	TArray<float> generation_weights;

	TArray<TArray<APathfindingTerrain*>> level;

	TSubclassOf<APathfindingTerrain> get_random_tile_type();
	
	void generate_level(int rows, int columns);
};
