// Fill out your copyright notice in the Description page of Project Settings.


#include "Frame.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Picture.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteComponent.h"
#include "Paper2DModule.h"
//#include "Paper"
// Sets default values
AFrame::AFrame()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFrame::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFrame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFrame::SpawnPicture()
{
	auto Picture = GetWorld()->SpawnActor<APicture>(Picture_BP);
	if (Picture->IsHorizontal)
		Frame->AddLocalRotation(FRotator(90, 0, 0));
	Picture->SetActorTransform(ArrowComponent->GetComponentTransform());
	FRotator Rotation = ArrowComponent->GetComponentRotation();
	FRotator PictureRotation = Picture->GetActorRotation();
	Picture->SetActorRotation(FRotator(Rotation));
	Picture->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
	Picture->SetScale();
	Picture->SetCost();
}

void AFrame::SetFrame(UArrowComponent* ArrowToSet, USceneComponent* FrameToSet)
{
	ArrowComponent = ArrowToSet;
	Frame = FrameToSet;
}

void AFrame::ChangeSpriteColor()
{
	Sprite->SetSpriteColor(FLinearColor::Red);
}

void AFrame::Color(FLinearColor Color)
{
	Sprite->SetSpriteColor(Color);
}

void AFrame::SetPictureSprite(UPaperSpriteComponent* SpriteToSet)
{
	Sprite=SpriteToSet;
}

