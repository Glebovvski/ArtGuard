// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Picture.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Bonus.h"
#include "GI_ArtGuard.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGuard::AGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Loudness = 1;
	SneakLoudness = Loudness * 0.5;
}

void AGuard::SetCatchOverlappingBox(UStaticMeshComponent* Box)
{
	CatchBox = Box;
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

void AGuard::ApplyBonus(ABonus* Bonus)
{
	float percent = Bonus->BonusPercent / 100;

	switch (Bonus->BonusType)
	{
	case (EBonusType::SneakLoudness):
		SneakLoudness -= SneakLoudness * percent;
		break;
	case (EBonusType::Loudness):
		Loudness -= Loudness * percent;
		break;
	case (EBonusType::CatchCone):
		CatchConeRadius += CatchConeRadius * percent;
		break;
	case (EBonusType::RadiusVisibility):
		VisibilityRadius += VisibilityRadius * percent;
		break;
	case (EBonusType::SneakSpeed):
		SneakSpeed += SneakSpeed * percent;
		break;
	case (EBonusType::WalkSpeed):
		WalkSpeed += WalkSpeed * percent;
		break;
	default:
		break;
	}
	auto GI = Cast<UGI_ArtGuard>(GetWorld()->GetGameInstance());
	GI->SaveGuardStats(WalkSpeed, SneakSpeed, VisibilityRadius, Loudness, SneakLoudness, CatchConeRadius);
}

void AGuard::ApplyPenalty(ABonus* Bonus)
{
	float percent = Bonus->BonusPercent / 100;
	
	switch (Bonus->BonusType)
	{
	case (EBonusType::SneakLoudness):
		SneakLoudness += SneakLoudness * percent;
		break;
	case (EBonusType::Loudness):
		Loudness += Loudness * percent;
		break;
	case (EBonusType::CatchCone):
		CatchConeRadius -= CatchConeRadius * percent;
		break;
	case (EBonusType::RadiusVisibility):
		VisibilityRadius -= VisibilityRadius * percent;
		break;
	case (EBonusType::SneakSpeed):
		SneakSpeed -= SneakSpeed * percent;
		break;
	case (EBonusType::WalkSpeed):
		WalkSpeed -= WalkSpeed * percent;
		break;
	default:
		break;
	}
	auto GI = Cast<UGI_ArtGuard>(GetWorld()->GetGameInstance());
	GI->SaveGuardStats(WalkSpeed, SneakSpeed, VisibilityRadius, Loudness, SneakLoudness, CatchConeRadius);
}

void AGuard::SetupGuardStats()
{
	auto GI = Cast<UGI_ArtGuard>(UGameplayStatics::GetGameInstance(GetWorld()));//(GetWorld()->GetGameInstance());
	if (GI)
	{
		WalkSpeed = GI->GuardWalkSpeed;//600;
		SneakSpeed = GI->GuardSneakSpeed;//200;
		VisibilityRadius = GI->GuardVisibilityRadius;//2000;
		Loudness = GI->GuardLoudness;//1;
		SneakLoudness = GI->GuardSneakLoudness;//0.7;
		CatchConeRadius = GI->GuardCatchConeRadius;//3;
	}
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	//SetupGuardStats();

	if (CatchBox)
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