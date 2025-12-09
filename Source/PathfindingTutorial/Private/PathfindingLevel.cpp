// Fill out your copyright notice in the Description page of Project Settings.



#include "PathfindingLevel.h"
#include "NavigationSystem.h"
// Sets default values
APathfindingLevel::APathfindingLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathfindingLevel::BeginPlay()
{
	Super::BeginPlay();
	generate_level(level_rows, level_columns);
}

// Called every frame
void APathfindingLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TSubclassOf<APathfindingTerrain> APathfindingLevel::get_random_tile_type()
{
	float total_weight = 0.0f;
	for (int i = 0; i < generation_weights.Num(); i++)
	{
		total_weight += generation_weights[i];
	}
	float random_value = FMath::RandRange(0.0f, total_weight);
	for (int i = 0; i < possible_tiles.Num(); i++)
	{
		random_value -= generation_weights[i];
		if (random_value <= 0.0f)
		{
			return possible_tiles[i];
		}
	}
	return possible_tiles.Last();
	
}

TArray<APathfindingTerrain*> APathfindingLevel::get_neighbors(APathfindingTerrain* tile)
{
	TArray<APathfindingTerrain*> neighbors;
	for (std::pair<int, int> direction : grid_directions)
	{
		if (direction.first > 0 and direction.first < level_rows)
		{
			if (direction.second > 0 and direction.second < level_columns)
			{
				if (level[direction.first][direction.second]->walkable)
				{
					neighbors.Add(level[direction.first][direction.second]);
				}
			}
		}
	}
	return neighbors;
}

float APathfindingLevel::linear_distance_between(APathfindingTerrain* first, APathfindingTerrain* second)
{
	return (first->GetActorLocation()-second->GetActorLocation()).Size();
}

void APathfindingLevel::generate_level(int rows, int columns)
{
	for (int row_num = 0; row_num < rows; row_num++)
	{
		TArray<APathfindingTerrain*> row_tiles;
		for (int column_num = 0; column_num < columns; column_num++)
		{
			TSubclassOf<APathfindingTerrain> tile_type;
			if (row_num == 0 and column_num == 0)
			{
				tile_type = start_tile_type;
			}
			else if (row_num == rows - 1 and column_num == columns - 1)
			{
				tile_type = goal_tile_type;
			}
			else
			{
				tile_type = get_random_tile_type();
			}
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(column_num * 100, row_num * 100, 0.0f));

			APathfindingTerrain* tile_in_world = GetWorld()->SpawnActorDeferred<APathfindingTerrain>(tile_type, SpawnTransform);
			tile_in_world->FinishSpawning(SpawnTransform);
			row_tiles.Add(tile_in_world);
		}
		level.Add(row_tiles);
	}
	
	UNavigationSystemV1* NavSys =
		FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (NavSys)
	{
		NavSys->Build();
	}
}

