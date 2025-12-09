// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PathfindingTerrain.h"
#include "NavMesh/NavMeshBoundsVolume.h"
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
	int level_rows;

	UPROPERTY(EditAnywhere)
	int level_columns;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APathfindingTerrain>> possible_tiles;
	
	UPROPERTY(EditAnywhere)
	TArray<float> generation_weights;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APathfindingTerrain> start_tile_type;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APathfindingTerrain> goal_tile_type;
	
	TArray<TArray<APathfindingTerrain*>> level;

	TSubclassOf<APathfindingTerrain> get_random_tile_type();

	TArray<std::pair<int, int>> grid_directions = {
		std::make_pair(1, -1),
		std::make_pair(1, 0),
		std::make_pair(1, 1),
		std::make_pair(0, -1),
		std::make_pair(0, 1),
		std::make_pair(-1, -1),
		std::make_pair(-1, 0),
		std::make_pair(-1, 1),
		
	};
	
	TArray<APathfindingTerrain*> get_neighbors(APathfindingTerrain* tile);

	float linear_distance_between(APathfindingTerrain* first, APathfindingTerrain* second);
	
	void generate_level(int rows, int columns);
};
