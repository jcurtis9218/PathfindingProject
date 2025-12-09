// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingController.h"
#include <queue>
#include <map>

APathfindingController::APathfindingController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APathfindingController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	move_along_path(DeltaTime);
}

void APathfindingController::move_along_path(float DeltaTime)
{
	if (path.Num() == 0 or current_index >= path.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("No path found"));
		return;
	}
	APawn* controlled_pawn = GetPawn();
	if (not controlled_pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn Found"));
		return;
	}
	FVector target_location = path[current_index]->GetActorLocation();
	FVector movement_direction = (target_location-controlled_pawn->GetActorLocation()).GetSafeNormal();

	controlled_pawn->AddMovementInput(movement_direction, movement_speed*DeltaTime);
	if ((controlled_pawn->GetActorLocation()-target_location).Size() < target_tolerance)
	{
		current_index++;
	}
}

struct FTilePriority
{
	float priority;
	APathfindingTerrain* tile;

	bool operator<(FTilePriority const& other) const
	{
		return priority < other.priority;
	}
	
};



uint32 APathfindingController::GetTypeHash(const ComparableTile& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(ComparableTile));
	return Hash;
}

void APathfindingController::generate_path(APathfindingLevel* level, APathfindingTerrain* start, APathfindingTerrain* goal)
{
	ComparableTile start_tile = {start};
	TArray<ComparableTile> found_path;
	UE_LOG(LogTemp, Log, TEXT("Generating Path"));
	std::priority_queue<FTilePriority> frontier;
	frontier.push({0.0, start});
	UE_LOG(LogTemp, Error, TEXT("Added Start to Frontier"));
	TMap<ComparableTile, ComparableTile> previous_tile;
	TMap<ComparableTile, float> cost_of_tile;
	UE_LOG(LogTemp, Error, TEXT("About to Set Start Cost"));
	cost_of_tile[start_tile] = 0.0;
	UE_LOG(LogTemp, Error, TEXT("Set Start Cost to 0"));
	UE_LOG(LogTemp, Error, TEXT("Starting Pathfinding"));
	while (!frontier.empty())
	{
		APathfindingTerrain* current_tile = frontier.top().tile;
		ComparableTile current_tile_comparable = {current_tile};
		UE_LOG(LogTemp, Error, TEXT("%f, %f"), level->get_grid_location(current_tile).X, level->get_grid_location(current_tile).Y);
		frontier.pop();
		if (current_tile == goal)
		{
			break;
		}
		for (APathfindingTerrain* neighbor_tile : level->get_neighbors(current_tile))
		{
			float new_cost = cost_of_tile[current_tile_comparable] + neighbor_tile->movement_cost;
			ComparableTile neighbor_comparable = {neighbor_tile};
			if ((not cost_of_tile.Contains(neighbor_comparable)) or new_cost < (cost_of_tile[neighbor_comparable]))
			{
				cost_of_tile[neighbor_comparable] = new_cost;
				float priority = new_cost + level->linear_distance_between(goal, neighbor_tile);
				frontier.push({priority, neighbor_tile});
				ComparableTile current_comparable = {current_tile};
				previous_tile[neighbor_comparable] = current_comparable;
			}
		}
	}
	APathfindingTerrain* retrace_current = goal;
	while (not (retrace_current == start))
	{
		ComparableTile current_comparable = {retrace_current};
		found_path.Add(current_comparable);
		retrace_current = previous_tile[current_comparable].tile;
		if (retrace_current == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Empty Array"))
			break;
		}
	}
	path = TArray<APathfindingTerrain*>();
	for (int i = found_path.Num() - 1; i >= 0; i--)
	{
		path.Add(found_path[i].tile);
	}
	UE_LOG(LogTemp, Error, TEXT("Path:"));
	for (APathfindingTerrain* tile : path)
	{
		UE_LOG(LogTemp, Error, TEXT("%f, %f"), tile->GetActorLocation().X, tile->GetActorLocation().Y);
	}
	
}
