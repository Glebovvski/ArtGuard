// Fill out your copyright notice in the Description page of Project Settings.


#include "Robber.h"
#include "Components/BoxComponent.h"
#include "Picture.h"
#include "Components/SphereComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
ARobber::ARobber()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ShouldEscape = false;
}

void ARobber::SetStealOverlappings(UBoxComponent* Box, USphereComponent* Sphere)
{
	StealOverlapComponent = Box;
	SearchRadius = Sphere;
}

void ARobber::SetGuardOverlappings(USphereComponent* Sphere)
{
	GuardRadius = Sphere;
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
		APicture* Pic = Cast<APicture>(OtherActor);
		if (!Pic->Assessed)
		{
			SeenPictures.Add(Pic);
			SetPictureFound();
		}
	}
}

void ARobber::OnOverlapSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Picture"))
	{
		SeenPictures.Remove(Cast<APicture>(OtherActor));
		SetPictureFound();
	}
}

void ARobber::OnOverlapGuardBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor->ActorHasTag("Playable"))
	{
		SetShouldEscape(true);
		//SetEscape();
	}
}

bool ARobber::GetShouldEscape()
{
	return ShouldEscape;
}

// Called when the game starts or when spawned
void ARobber::BeginPlay()
{
	Super::BeginPlay();
	StolenMoney = 0;
	if (StealOverlapComponent)
	{
		StealOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ARobber::OnOverlapBegin);
		StealOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ARobber::OnOverlapEnd);
	}
	if (SearchRadius)
	{
		SearchRadius->OnComponentBeginOverlap.AddDynamic(this, &ARobber::OnOverlapSphereBegin);
		SearchRadius->OnComponentEndOverlap.AddDynamic(this, &ARobber::OnOverlapSphereEnd);
	}
	if (GuardRadius)
	{
		GuardRadius->OnComponentBeginOverlap.AddDynamic(this, &ARobber::OnOverlapGuardBegin);
	}

}

// Called every frame
void ARobber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARobber::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

TArray<APicture*> ARobber::GetSeenPictures()
{
	return SeenPictures;
}

void ARobber::Steal()
{
	if (PictureToSteal && PictureToSteal->CanSteal())
	{
		StolenMoney += PictureToSteal->GetCost();
		PictureToSteal->Steal();
		PicturesStolen++;
		PictureToSteal = nullptr;
	}
}

APicture* ARobber::GetPictureToSteal()
{
	return PictureToSteal;
}

void ARobber::SetShouldEscape(bool Escape)
{
	ShouldEscape = Escape;
}

UAIPerceptionComponent* ARobber::GetPerception()
{

	//auto Controller = UAIBlueprintHelperLibrary::GetAIController(this);
	return UAIBlueprintHelperLibrary::GetAIController(this)->GetAIPerceptionComponent();//Controller->GetPerceptionComponent();
}

bool ARobber::AssessPicture()
{
	if (PictureToSteal)
	{
		PictureToSteal->Assessed = true;
		SeenPictures.Remove(PictureToSteal);
		if (PicturesStolen > 0)
		{
			int PictureCost = PictureToSteal->GetCost();
			float Risk = (StolenMoney / PicturesStolen) / 2;
			return PictureCost > Risk ? true : false;
		}
		return true;
	}
	return false;
}

int ARobber::GetStolenMoney()
{
	return StolenMoney;
}

int ARobber::GetPicturesStolen()
{
	return PicturesStolen;
}

void ARobber::SetEscape_Implementation()
{
}

void ARobber::SetPictureFound_Implementation()
{
}

