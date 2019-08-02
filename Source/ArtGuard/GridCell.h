// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ARTGUARD_API GridCell
{
public:
	int Index;
	class ACell* Cell;
	
	GridCell();
	~GridCell();
};
