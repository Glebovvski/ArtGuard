// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Picture.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"

// Sets default values
AGuard::AGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Loudness=1;
	SneakLoudness=Loudness*0.5;
}

void AGuard::SetCatchOverlappingBox(UStaticMeshComponent* Box)
{
	CatchBox=Box;
}

void AGuard::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("Robber"))
	{
		Chase();
	}
}

void AGuard::Catch_Implementation()
{
}

void AGuard::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("Robber"))
	{
		StopChase();
	}
}

void AGuard::Chase_Implementation()
{
}

void AGuard::StopChase_Implementation()
{
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	if(CatchBox)
	{
		CatchBox->OnComponentBeginOverlap.AddDynamic(this, &AGuard::OnOverlapBegin);
		CatchBox->OnComponentEndOverlap.AddDynamic(this, &AGuard::OnOverlapEnd);
	}
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

UAIPerceptionComponent* AGuard::GetPerception()
{
	return UAIBlueprintHelperLibrary::GetAIController(this)->GetAIPerceptionComponent();
}