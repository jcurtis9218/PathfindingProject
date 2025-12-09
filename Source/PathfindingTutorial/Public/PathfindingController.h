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
	APathfindingController();
public:
	virtual void Tick(float DeltaTime) override;
	
	TArray<APathfindingTerrain*> path;
	int current_index = 0;

	UPROPERTY(EditAnywhere)
	float movement_speed;
	
	UPROPERTY(EditAnywhere)
	float target_tolerance;

	struct ComparableTile
	{
		APathfindingTerrain* tile;

		bool operator<(ComparableTile const& other) const
		{
			return tile->GetActorLocation().Size() < other.tile->GetActorLocation().Size();
		}

		bool operator==(const ComparableTile& other) const
		{
			return tile == other.tile;
		}
	};
	
	uint32 GetTypeHash(const ComparableTile& Thing);
	
	void move_along_path(float DeltaTime);
	void generate_path(APathfindingLevel* level, APathfindingTerrain* start, APathfindingTerrain* goal);
};
