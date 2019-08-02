// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtCanvas.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"

// Sets default values
AArtCanvas::AArtCanvas()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArtCanvas::BeginPlay()
{
	Super::BeginPlay();
	//UMaterial* material;
	//UMaterialInterface* MaterialInt= UMaterialInterface(material);
	//Canvas->SetMaterial(0, MaterialInt);
}

// Called every frame
void AArtCanvas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

