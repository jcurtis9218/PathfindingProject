// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PathfindingLevel.h"
#include "PathfindingTerrain.h"
#include "PathfindingController.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDINGTUTORIAL_API APathfindingController : public AAIController
{
	GENERATED_BODY()
public:
	TArray<APathfindingTerrain*> path;
	int current_index = 0;

	void move_along_path(float DeltaTime);
	void generate_path(APathfindingLevel* level, APathfindingTerrain* start, APathfindingTerrain* goal);
};
