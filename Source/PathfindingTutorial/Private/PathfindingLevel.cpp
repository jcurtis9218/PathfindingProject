// Fill out your copyright notice in the Description page of Project Settings.



#include "PathfindingLevel.h"
#include "NavigationSystem.h"
#include "PathfindingController.h"
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
	FTimerHandle timer_handle;
	GetWorldTimerManager().SetTimer(timer_handle, [this]() {on_level_ready();}, 0.5f, false);
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

FVector APathfindingLevel::get_grid_location(APathfindingTerrain* tile)
{
	for (int row = 0; row < level_rows; row++)
	{
		for (int column = 0; column < level_columns; column++)
		{
			if (level[row][column] == tile)
			{
				return FVector(row, column, 0);
			}
		}
	}
	return FVector(0, 0, 0);
}

TArray<APathfindingTerrain*> APathfindingLevel::get_neighbors(APathfindingTerrain* tile)
{
	TArray<APathfindingTerrain*> neighbors;
	FVector tile_location = get_grid_location(tile);
	for (FVector direction : grid_directions)
	{
		FVector neighbor_location = tile_location + direction;
		if (neighbor_location.X > 0 and neighbor_location.X < level_rows)
		{
			if (neighbor_location.Y > 0 and neighbor_location.Y < level_columns)
			{
				if (level[neighbor_location.X][neighbor_location.Y]->walkable)
				{
					neighbors.Add(level[static_cast<int>(neighbor_location.X)][static_cast<int>(neighbor_location.Y)]);
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

void APathfindingLevel::on_level_ready()
{
	UE_LOG(LogTemp, Error, TEXT("Looking For Pawn"));
	if (not pawn_class)
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn Found"));
		return;
	}
	FVector spawn_location = GetActorLocation()+FVector(0, 0, 25);
	pawn_in_world = GetWorld()->SpawnActor<APawn>(pawn_class, spawn_location, FRotator(0, 0, 0));
	APathfindingController* controller_in_world = Cast<APathfindingController>(pawn_in_world->GetController());
	UE_LOG(LogTemp, Error, TEXT("Looking For Controller"));
	if (controller_in_world)
	{
		UE_LOG(LogTemp, Error, TEXT("Found Controller"));
		controller_in_world->generate_path(this, level[0][0], level[level_rows-1][level_columns-1]);
	}
}

