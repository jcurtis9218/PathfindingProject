// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingLevel.h"

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

void APathfindingLevel::generate_level(int rows, int columns)
{
	for (int row_num = 0; row_num < rows; row_num++)
	{
		TArray<APathfindingTerrain*> row_tiles;
		for (int column_num = 0; column_num < columns; column_num++)
		{
			TSubclassOf<APathfindingTerrain> tile_type = get_random_tile_type();
			APathfindingTerrain* tile_in_world = GetWorld()->SpawnActor<APathfindingTerrain>(tile_type);
			tile_in_world->SetActorLocation(FVector(column_num*100, row_num*100, 0.0f));
			row_tiles.Add(tile_in_world);
		}
		level.Add(row_tiles);
	}
}

