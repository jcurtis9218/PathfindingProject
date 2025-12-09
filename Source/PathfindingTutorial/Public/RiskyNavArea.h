// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "RiskyNavArea.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDINGTUTORIAL_API URiskyNavArea : public UNavArea
{
	GENERATED_BODY()

public:
	URiskyNavArea()
	{
		DefaultCost = 6.0f;          // Multiplies path length
		FixedAreaEnteringCost = 0.f; // Optional flat penalty
	}
};
