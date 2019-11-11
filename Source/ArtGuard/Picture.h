// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Picture.generated.h"

UENUM()
enum MaterialAlignments
{
	VERTICAL = 0,
	HORIZONTAL = 1
};

UCLASS()
class ARTGUARD_API APicture : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APicture();

	void SetScale();
	void RetryApply();
	void ApplyMaterial();

	bool IsStolen=false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SetPlane(UStaticMeshComponent* PlaneToSet);

	UPROPERTY(BlueprintReadOnly, Category = "Spawn")
		class AArtGuardGameMode* GameMode;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		bool IsSet = false;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		bool IsHorizontal;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		class UMaterialInstance* Material;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		class UStaticMeshComponent* Plane;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float RotationX;

	UFUNCTION(BlueprintCallable, Category="Steal")
	int GetCost();

	UFUNCTION(BlueprintCallable, Category="Steal")
	bool CanSteal();
	UFUNCTION(BlueprintCallable, Category="Steal")
	void Steal();

	bool Assessed = false;

	void SetCost();

	
	UPROPERTY(BlueprintReadOnly, Category="Spawn")
	bool IsFenceVisible;

	UPROPERTY(VisibleAnywhere, Category="Cost")
	int Cost = 0;
private:
	TArray<UMaterialInstance*> Materials;
};
