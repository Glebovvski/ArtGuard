// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Robber.generated.h"

class UBoxComponent;
class USphereComponent;
class APicture;

UCLASS()
class ARTGUARD_API ARobber : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobber();

	UFUNCTION(BlueprintCallable, Category="Steal")
	void SetStealOverlappings(UBoxComponent* Box, USphereComponent* Sphere);
	UFUNCTION(BlueprintCallable, Category="Steal")
	void SetGuardOverlappings(USphereComponent* Sphere);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapSphereBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapGuardBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool ShouldEscape=false;
	//UFUNCTION()
	//void OnOverlapSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Search")
	TArray<APicture*> GetSeenPictures();
	UPROPERTY(VisibleAnywhere, Category="Search")
	TArray<APicture*> SeenPictures;

	UFUNCTION(BlueprintCallable, Category="Stealing")
	void Steal();
	UFUNCTION(BlueprintCallable, Category="Steal")
	APicture* GetPictureToSteal();
private:
	UBoxComponent* StealOverlapComponent;
	USphereComponent* SearchRadius;
	USphereComponent* GuardRadius;

	APicture* PictureToSteal;

};
