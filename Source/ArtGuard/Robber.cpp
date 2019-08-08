// Fill out your copyright notice in the Description page of Project Settings.


#include "Robber.h"
#include "Components/BoxComponent.h"
#include "Picture.h"
#include "Components/SphereComponent.h"

// Sets default values
ARobber::ARobber()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARobber::SetStealOverlappings(UBoxComponent* Box, USphereComponent* Sphere)
{
	StealOverlapComponent = Box;
	SearchRadius = Sphere;
}

void ARobber::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Picture"))
	{
		PictureToSteal = Cast<APicture>(OtherActor);
	}
}

void ARobber::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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

void ARobber::OnOverlapSphereBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Picture"))
	{
		SeenPictures.Add(Cast<APicture>(OtherActor));
	}
}

void ARobber::OnOverlapSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Picture"))
	{
		SeenPictures.Remove(Cast<APicture>(OtherActor));
	}
}

// Called when the game starts or when spawned
void ARobber::BeginPlay()
{
	Super::BeginPlay();
	if (StealOverlapComponent)
	{
		StealOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ARobber::OnOverlapBegin);
		StealOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ARobber::OnOverlapEnd);
	}
	if(SearchRadius)
	{
		SearchRadius->OnComponentBeginOverlap.AddDynamic(this, &ARobber::OnOverlapSphereBegin);
		SearchRadius->OnComponentEndOverlap.AddDynamic(this, &ARobber::OnOverlapSphereEnd);
	}
}

// Called every frame
void ARobber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<APicture*> ARobber::GetSeenPictures()
{
	return SeenPictures;
}

void ARobber::Steal()
{
	if(PictureToSteal && PictureToSteal->CanSteal())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PictureToSteal->GetName());
		PictureToSteal->Steal();
	}
}

APicture* ARobber::GetPictureToSteal()
{
	return PictureToSteal;
}

