// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Picture.h"

// Sets default values
AGuard::AGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGuard::SetStealOverlappingBox(UBoxComponent* Box)
{
	StealOverlapComponent = Box;
}

void AGuard::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Picture"))
	{
		PictureToSteal = Cast<APicture>(OtherActor);
	}
}

void AGuard::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Picture"))
	{
		if (Cast<APicture>(OtherActor) == PictureToSteal)
		{
			PictureToSteal = nullptr;
		}
	}
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	if(StealOverlapComponent)
	{
		StealOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AGuard::OnOverlapBegin);
		StealOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &AGuard::OnOverlapEnd);
	}
	InputComponent->BindAction("StealPicture", IE_Pressed, this, &AGuard::Steal);
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGuard::Steal()
{
	if(PictureToSteal && PictureToSteal->CanSteal())
	{
		PictureToSteal->Steal();
	}
}