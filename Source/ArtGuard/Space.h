// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "Interface.h"
#include "Space.generated.h"

/**
 * 
 */
UINTERFACE()
class ARTGUARD_API USpace : public UInterface
{
	GENERATED_BODY()
	//GENERATED_UINTERFACE_BODY()
	
};

class ISpace
{
	GENERATED_BODY()
	//GENERATED_IINTERFACE_BODY()

public:
	FVector Location;

//	UPROPERTY(VisibleInstanceOnly)
	float Height;
	//UPROPERTY(VisibleInstanceOnly)
	float Width;

};