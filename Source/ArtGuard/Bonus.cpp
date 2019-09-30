// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"

Bonus::Bonus()
{
}

Bonus::Bonus(::EBonusAddition Addition, EBonusType Type, FString Text)
{
	BonusAddition=Addition;
	BonusType=Type;
	BonusText=Text;
}

void Bonus::SetPercent(double Percent)
{
	BonusPercent=Percent;
}

Bonus::~Bonus()
{
}
