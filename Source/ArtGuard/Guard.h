// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

class UBoxComponent;
class APicture;
class UAIPerceptionComponent;

UCLASS()
class ARTGUARD_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

	UFUNCTION(BlueprintCallable, Category="Steal")
	void SetCatchOverlappingBox(/*UBoxComponent**/UStaticMeshComponent* Box);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Endgame")
	void Catch();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Endgame")
	void Chase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Endgame")
	void StopChase();
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float WalkSpeed;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float SneakSpeed;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float VisibilityRadius;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float Loudness;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float SneakLoudness;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float CatchConeRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UAIPerceptionComponent* GetPerception();
private:
	//UBoxComponent* CatchBox;
	UStaticMeshComponent* CatchBox;
};
