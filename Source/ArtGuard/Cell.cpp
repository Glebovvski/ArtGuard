// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "Components/BoxComponent.h"
#include "Room.h"

// Sets default values
ACell::ACell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACell::SetPlane(UStaticMeshComponent* PlaneToSet)
{
	Plane = PlaneToSet;
}

void ACell::SetCollisionBox(UBoxComponent* BoxToSet)
{
	CollisionCheck = BoxToSet;
}

void ACell::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("Room"))//OtherActor->ActorHasTag("Room"))
	{
		IsUsed = true;
		//UE_LOG(LogTemp, Warning, TEXT("OVERLAPPING %s with %s"), *OverlappedComp->GetOwner()->GetName(), *OtherActor->GetName());
	}

}

void ACell::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Room"))
	{
		IsUsed = false;
		//UE_LOG(LogTemp, Warning, TEXT("END OVERLAP %s with %s"), *OverlappedComp->GetOwner()->GetName(), *OtherActor->GetName());
	}
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();

	CollisionCheck->OnComponentBeginOverlap.AddDynamic(this, &ACell::OnOverlapBegin);
	//CollisionCheck->OnComponentEndOverlap.AddDynamic(this, &ACell::OnOverlapEnd);
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

