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
		return;
	}
	APawn* controlled_pawn = GetPawn();
	if (not controlled_pawn)
	{
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
	std::priority_queue<std::pair<float, APathfindingTerrain*>> frontier;
	frontier.push(std::make_pair(0, start));
	std::map<APathfindingTerrain*, APathfindingTerrain*> previous_tile;
	std::map<APathfindingTerrain*, float> cost_of_tile;
	previous_tile[start] = nullptr;
	cost_of_tile[start] = 0;

	while (!frontier.empty())
	{
		APathfindingTerrain* current_tile = frontier.top().second;
		if (current_tile == goal)
		{
			break;
		}
		for (APathfindingTerrain* neighbor_tile : level->get_neighbors(current_tile))
		{
			float new_cost = cost_of_tile[current_tile] + neighbor_tile->movement_cost;
			if ((not cost_of_tile.contains(neighbor_tile)) or new_cost < cost_of_tile[neighbor_tile])
			{
				cost_of_tile[neighbor_tile] = new_cost;
				float priority = new_cost + level->linear_distance_between(goal, neighbor_tile);
				frontier.push(std::make_pair(priority, current_tile));
				previous_tile[neighbor_tile] = current_tile;
			}
		}
	}
	
}
