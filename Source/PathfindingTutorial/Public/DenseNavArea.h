// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "DenseNavArea.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDINGTUTORIAL_API UDenseNavArea : public UNavArea
{
	GENERATED_BODY()

public:
	UDenseNavArea()
	{
		DefaultCost = 2.0f;          // Multiplies path length
		FixedAreaEnteringCost = 0.f; // Optional flat penalty
	}
};
