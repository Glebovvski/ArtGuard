// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"

// Sets default values
ABonus::ABonus()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

ABonus* ABonus::BonusInit(EBonusAddition Addition, EBonusType Type, FString Text)
{
	ABonus* b = NewObject<ABonus>();
	
	b->BonusAddition = Addition;
	b->BonusType = Type;
	b->BonusText = Text;
	b->BonusPercent=FMath::RandRange(10,15);
	return b;
}

FString ABonus::GetBonusText()
{
	return BonusText;
}

float ABonus::GetBonusPercent()
{
	return BonusPercent;
}

// Called when the game starts or when spawned
void ABonus::BeginPlay()
{
	Super::BeginPlay();

}

//ABonus::BonusInit(::EBonusAddition Addition, EBonusType Type, FString Text)
//{
//	BonusAddition = Addition;
//	BonusType = Type;
//	BonusText = Text;
//}


void ABonus::SetPercent(double Percent)
{
	BonusPercent = Percent;
}


