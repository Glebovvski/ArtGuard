// Fill out your copyright notice in the Description page of Project Settings.


#include "Picture.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Components/StaticMeshComponent.h"
#include "ArtGuardGameMode.h"

// Sets default values
APicture::APicture()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APicture::SetScale()
{
	if (IsHorizontal)
		Plane->SetRelativeScale3D(FVector(1.25, 0.9, 1));
	else
		Plane->SetRelativeScale3D(FVector(0.9, 1.25, 1));
}

// Called when the game starts or when spawned
void APicture::BeginPlay()
{
	Super::BeginPlay();

	if (Plane != nullptr)
	{
		GameMode = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
		int Alignment = FMath::RandRange(VERTICAL, HORIZONTAL);

		switch (Alignment)
		{
		case VERTICAL:
			Materials = GameMode->GetVerticalMaterials();
			IsHorizontal = false;
			break;
		case HORIZONTAL:
			Materials = GameMode->GetHorizontalMaterials();
			IsHorizontal = true;
			break;
		default:
			Materials = GameMode->GetVerticalMaterials();
			IsHorizontal = false;
			break;
		}

		if (Materials.Num() > 0)
		{
			ApplyMaterial();
		}
		else
		{
			FString array = IsHorizontal ? "Horizontal" : "Vertical";
			UE_LOG(LogTemp, Warning, TEXT("used all mi at %s array"), *array);
			RetryApply();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PLANE IS MISSING"));
	}
}

void APicture::ApplyMaterial()
{
	int RandomIndex = FMath::RandRange(0, Materials.Num() - 1);
	Material = Materials[RandomIndex];
	Plane->SetMaterial(0, Material);
	//GameMode->RemoveUsedPictureFromArray(IsHorizontal, RandomIndex);
}


void APicture::RetryApply()
{
	IsHorizontal = !IsHorizontal;
	if (IsHorizontal)
		Materials = GameMode->GetHorizontalMaterials();
	else
		Materials = GameMode->GetVerticalMaterials();
	ApplyMaterial();
}

void APicture::SetPlane(UStaticMeshComponent* PlaneToSet)
{
	Plane = PlaneToSet;
}

// Called every frame
void APicture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

