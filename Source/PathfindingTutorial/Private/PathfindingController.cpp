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
	target_location.Z = controlled_pawn->GetActorLocation().Z;
	UE_LOG(LogTemp, Error, TEXT("Moving towards %f, %f"), target_location.X, target_location.Y);
	FVector movement_delta = target_location - controlled_pawn->GetActorLocation();
	FVector movement_direction = movement_delta.GetSafeNormal();
	UE_LOG(LogTemp, Error, TEXT("Movement Direction: %f, %f"), movement_direction.X, movement_direction.Y);

	controlled_pawn->AddMovementInput(movement_direction, 1);
	if ((controlled_pawn->GetActorLocation()-target_location).Size() < target_tolerance)
	{
		UE_LOG(LogTemp, Error, TEXT("Target %d Reached"), current_index)
		current_index++;
	}
}

void APathfindingController::generate_path(APathfindingLevel* level, APathfindingTerrain* start, APathfindingTerrain* goal)
{
	int start_tile_index = level->get_index_from_tile(start);
	TArray<int> found_path;
	std::priority_queue<std::pair<int, int>> frontier;
	frontier.push(std::make_pair(0, start_tile_index));
	std::map<int, int> previous_tile;
	std::map<int, float> cost_of_tile;
	cost_of_tile[start_tile_index] = 0.0;
	while (!frontier.empty())
	{
		int current_tile_index = frontier.top().second;
		APathfindingTerrain* current_tile = level->get_tile_by_index(current_tile_index);
		frontier.pop();
		if (current_tile == goal)
		{
			break;
		}
		TArray<APathfindingTerrain*> neighbors = level->get_neighbors(current_tile);
		for (int i = 0; i < neighbors.Num(); i++)
		{
			APathfindingTerrain* neighbor_tile = neighbors[i];
			int neighbor_tile_index = level->get_index_from_tile(neighbor_tile);
			float new_cost = cost_of_tile[current_tile_index] + neighbor_tile->movement_cost;
			if ((not cost_of_tile.contains(neighbor_tile_index)) or new_cost < (cost_of_tile[neighbor_tile_index]))
			{
				cost_of_tile[neighbor_tile_index] = new_cost;
				float priority = new_cost + level->linear_distance_between(goal, neighbor_tile);
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
			break;
		}
	}
	path = TArray<APathfindingTerrain*>();
	for (int i = found_path.Num() - 1; i >= 0; i--)
	{
		path.Add(level->get_tile_by_index(found_path[i]));
	}
	
}
