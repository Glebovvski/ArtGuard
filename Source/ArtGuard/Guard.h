// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

class UBoxComponent;
class APicture;

UCLASS()
class ARTGUARD_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

	UFUNCTION(BlueprintCallable, Category="Steal")
	void SetCatchOverlappingBox(UBoxComponent* Box);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Endgame")
	void Catch();
	//UFUNCTION()
	//void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Steal")
	APicture* GetPictureToSteal();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	float Loudness;
	float SneakLoudness;

	UFUNCTION(BlueprintCallable, Category="Setup")
	float GetLoudness();
	UFUNCTION(BlueprintCallable, Category="Setup")
	float GetSneakLoudness();

	//UBoxComponent* StealOverlapComponent;
	APicture* PictureToSteal;

	void Steal();

	UBoxComponent* CatchBox;
};
