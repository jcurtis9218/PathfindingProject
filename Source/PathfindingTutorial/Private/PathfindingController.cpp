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

void APathfindingController::generate_path(APathfindingLevel* level, APathfindingTerrain* start, APathfindingTerrain* goal)
{
	int start_tile_index = level->get_index_from_tile(start);
	TArray<int> found_path;
	UE_LOG(LogTemp, Log, TEXT("Generating Path"));
	std::priority_queue<std::pair<int, int>> frontier;
	frontier.push(std::make_pair(0, start_tile_index));
	UE_LOG(LogTemp, Error, TEXT("Added Start to Frontier"));
	std::map<int, int> previous_tile;
	std::map<int, float> cost_of_tile;
	UE_LOG(LogTemp, Error, TEXT("About to Set Start (%p) Cost in %p"), &start_tile_index, &cost_of_tile);
	cost_of_tile[start_tile_index] = 0.0;
	UE_LOG(LogTemp, Error, TEXT("Set Start Cost to 0"));
	UE_LOG(LogTemp, Error, TEXT("Starting Pathfinding"));
	while (!frontier.empty())
	{
		int current_tile_index = frontier.top().second;
		APathfindingTerrain* current_tile = level->get_tile_by_index(current_tile_index);
		UE_LOG(LogTemp, Error, TEXT("%f, %f"), level->get_grid_location(current_tile).X, level->get_grid_location(current_tile).Y);
		frontier.pop();
		if (current_tile == goal)
		{
			UE_LOG(LogTemp, Error, TEXT("\tGoal found"));
			break;
		}
		UE_LOG(LogTemp, Error, TEXT("\tFinding Neighbors"));
		TArray<APathfindingTerrain*> neighbors = level->get_neighbors(current_tile);
		UE_LOG(LogTemp, Error, TEXT("\t\tFound %d Neighbors"), neighbors.Num());
		for (int i = 0; i < neighbors.Num(); i++)
		{
			APathfindingTerrain* neighbor_tile = neighbors[i];
			int neighbor_tile_index = level->get_index_from_tile(neighbor_tile);
			UE_LOG(LogTemp, Error, TEXT("\t\t\tScanning Neighbor %d of %d (Index %d)"), i, neighbors.Num(), neighbor_tile_index);
			float new_cost = cost_of_tile[current_tile_index] + neighbor_tile->movement_cost;
			if ((not cost_of_tile.contains(neighbor_tile_index)) or new_cost < (cost_of_tile[neighbor_tile_index]))
			{
				cost_of_tile[neighbor_tile_index] = new_cost;
				float priority = new_cost + level->linear_distance_between(goal, neighbor_tile);
				UE_LOG(LogTemp, Error, TEXT("\t\t\t\tAdding %d to Frontier"), neighbor_tile_index)
				frontier.push(std::make_pair(-priority, neighbor_tile_index));
				previous_tile[neighbor_tile_index] = current_tile_index;
			}
		}
	}
	APathfindingTerrain* retrace_current = goal;
	while (not (retrace_current == start))
	{
		int current_tile_index = level->get_index_from_tile(retrace_current);
		found_path.Add(current_tile_index);
		retrace_current = level->get_tile_by_index(previous_tile[current_tile_index]);
		if (retrace_current == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Empty Array"))
			break;
		}
	}
	path = TArray<APathfindingTerrain*>();
	for (int i = found_path.Num() - 1; i >= 0; i--)
	{
		path.Add(level->get_tile_by_index(found_path[i]));
	}
	UE_LOG(LogTemp, Error, TEXT("Path:"));
	for (APathfindingTerrain* tile : path)
	{
		UE_LOG(LogTemp, Error, TEXT("%f, %f"), tile->GetActorLocation().X, tile->GetActorLocation().Y);
	}
	
}
