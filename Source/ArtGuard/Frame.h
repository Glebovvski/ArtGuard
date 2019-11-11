// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Frame.generated.h"

class UArrowComponent;
class APicture;
class UPaperSpriteComponent;

UCLASS()
class ARTGUARD_API AFrame : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFrame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Arrow")
	//class UArrowComponent* Arrow;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Arrow")
	//class USceneComponent* SceneRoot;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="Spawn")
	void SpawnPicture(bool IsFenceVisible);

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SetFrame(UArrowComponent* ArrowToSet, USceneComponent* FrameToSet);

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<APicture> Picture_BP;


	UFUNCTION(BlueprintCallable, Category="Sprite")
	void SetPictureSprite(UPaperSpriteComponent* SpriteToSet);

	void ChangeSpriteColor();
	void Color(FLinearColor Color);
private:
	
	UPaperSpriteComponent* Sprite;
	UPROPERTY(EditDefaultsOnly, Category="Frame")
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditDefaultsOnly, Category="Frame")
	USceneComponent* Frame;
};
