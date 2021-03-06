// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Robber.generated.h"

class UBoxComponent;
class USphereComponent;
class APicture;
class UAIPerceptionComponent;
class ABonus;

UCLASS()
class ARTGUARD_API ARobber : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARobber();

	UFUNCTION(BlueprintCallable, Category = "Steal")
		void SetStealOverlappings(UBoxComponent* Box, USphereComponent* Sphere);
	UFUNCTION(BlueprintCallable, Category = "Steal")
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

	bool ShouldEscape = false;

	UFUNCTION(BlueprintCallable, Category = "Steal")
		bool GetShouldEscape();

	UFUNCTION(BlueprintCallable,Category="Stop")
	void Stop();
	//UFUNCTION()
	//void OnOverlapSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Search")
		TArray<APicture*> GetSeenPictures();
	UPROPERTY(VisibleAnywhere, Category = "Search")
		TArray<APicture*> SeenPictures;

	UFUNCTION(BlueprintCallable, Category = "Stealing")
		int Steal();
	UFUNCTION(BlueprintCallable, Category = "Steal")
		APicture* GetPictureToSteal();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stealing")
		void SetPictureFound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stealing")
		void SetEscape();

	UFUNCTION(BlueprintCallable, Category = "Steal")
		void SetShouldEscape(bool Escape);

	UFUNCTION(BlueprintCallable, Category="Steal")
	bool AssessPicture();

	UFUNCTION(BlueprintCallable, Category="Steal")
	float GetRiskAssessment();

	UFUNCTION(BlueprintCallable, Category="Steal")
	FColor GetColorOfRisk(int Risk);

	UFUNCTION(BlueprintCallable, Category="Endgame")
	bool IsEnoughStole();

	int TotalMoney;
	int TotalPictures;

	int GetStolenMoney();
	UFUNCTION(BlueprintCallable, Category="Steal")
	int GetPicturesStolen();

	UAIPerceptionComponent* GetPerception();

	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float VisibilityRadius;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float WalkSpeed;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float Loudness;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float CatchConeRadius;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float StealSpeed;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	bool VisibleExits;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	bool EnemyVisibility;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float CatchSpeed;
	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetupRobberStats();

	void ApplyBonus(ABonus* Bonus);
	void ApplyPenalty(ABonus* Bonus);
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	FString PenaltyText;

private:
	UBoxComponent* StealOverlapComponent;
	USphereComponent* SearchRadius;
	USphereComponent* GuardRadius;

	int StolenMoney = 0;
	int PicturesStolen=0;


	APicture* PictureToSteal;
};
