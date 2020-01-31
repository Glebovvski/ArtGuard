// Fill out your copyright notice in the Description page of Project Settings.


#include "Area.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ArtGuardGameMode.h"
#include "Room.h"
#include "Kismet/GameplayStatics.h"
#include "Exit.h"
#include "Wall.h"

// Sets default values
AArea::AArea()//(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MIN_AREA_SIZE = 20;

	MaxHeight = 60;
	MaxHeight = 60;
	MinWidth = 22;
	MinHeight = 22;

	Visited = false;

	LeftAreaChild = nullptr;
	RightAreaChild = nullptr;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(FName("Box"));
	SetRootComponent(Box);
	UpCollision = CreateDefaultSubobject<UBoxComponent>(FName("UpCollision")); //<UBoxComponent>(this, TEXT("UpCollision"));
	DownCollision = CreateDefaultSubobject<UBoxComponent>(FName("DownCollision")); //ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("DownCollision"));
	RightCollision = CreateDefaultSubobject<UBoxComponent>(FName("RightCollision")); //ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("RightCollision"));
	LeftCollision = CreateDefaultSubobject<UBoxComponent>(FName("LeftCollision")); //ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("LeftCollision"));

	UpCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	DownCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	RightCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	LeftCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);

	FTransform UpTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 105, 0),
		FVector(0.1, 0.5, 10)
	);
	FTransform DownTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, -105, 0),
		FVector(0.1, 0.5, 10)
	);

	FTransform RightTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(105, 0, 0),
		FVector(0.5, 0.1, 10)
	);

	FTransform LeftTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(-105, 0, 0),
		FVector(0.5, 0.1, 10)
	);
	UpCollision->SetWorldTransform(UpTransform);
	DownCollision->SetWorldTransform(DownTransform);
	RightCollision->SetWorldTransform(RightTransform);
	LeftCollision->SetWorldTransform(LeftTransform);

	UpCollision->SetGenerateOverlapEvents(true);
	DownCollision->SetGenerateOverlapEvents(true);
	RightCollision->SetGenerateOverlapEvents(true);
	LeftCollision->SetGenerateOverlapEvents(true);
}

void AArea::Init(int Width, int Height)
{
	SetActorScale3D(FVector(Width, Height, 1));
	this->Width = Width;
	this->Height = Height;
}

void AArea::SetBox(UStaticMeshComponent* BoxToSet)
{
	Box = BoxToSet;
	//UE_LOG(LogTemp, Warning, TEXT("BOX: %s , BOX_TO_SET:%s"), *Box->GetName(), *BoxToSet->GetName());
}


void AArea::SetCollisionBoxes(UBoxComponent* Up, UBoxComponent* Down, UBoxComponent* Left, UBoxComponent* Right)
{

	UpCollision = Up;
	//UpCollision->SetWorldScale3D(FVector(0.01, 5, 1));
	//UE_LOG(LogTemp, Warning, TEXT("UP COL: %s"), *UpCollision->GetName());
	DownCollision = Down;
	//DownCollision->SetWorldScale3D(FVector(0.01, 5, 1));
	LeftCollision = Left;
	//LeftCollision->SetWorldScale3D(FVector(5, 0.01, 1));
	RightCollision = Right;
	//RightCollision->SetWorldScale3D(FVector(5, 0.01, 1));

	CollisionArray.Add(UpCollision);
	CollisionArray.Add(DownCollision);
	CollisionArray.Add(LeftCollision);
	CollisionArray.Add(RightCollision);
}

void AArea::OnOverlapBeginUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("SWEEP: %s"), *SweepResult.ToString());
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)
	{
		UpRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("UP ROOM: %s"), *UpRoom->GetName());
	}
}

void AArea::OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("SWEEP: %s"), *SweepResult.ToString());
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)
	{
		DownRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("DOWN ROOM: %s"), *DownRoom->GetName());
	}
}

void AArea::OnOverlapBeginRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("SWEEP: %s"), *SweepResult.ToString());
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)
	{
		RightRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("RIGHT ROOM: %s"), *RightRoom->GetName());
	}
}

void AArea::OnOverlapBeginLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("SWEEP: %s"), *SweepResult.ToString());
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)
	{
		LeftRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("LEFT ROOM: %s"), *LeftRoom->GetName());
	}
}


// Called when the game starts or when spawned
void AArea::BeginPlay()
{
	Super::BeginPlay();
	//bGenerateOverlapEventsDuringLevelStreaming = true;
	Location = GetActorLocation();

	//if (DownCollision)
	//	DownCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginBottom);
	//else UE_LOG(LogTemp, Warning, TEXT("NO DOWN COL"));
	//if (UpCollision)
	//	UpCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginUp);
	//else UE_LOG(LogTemp, Warning, TEXT("NO UP COL"));
	//if (LeftCollision)
	//	LeftCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginLeft);
	//else UE_LOG(LogTemp, Warning, TEXT("NO LEFT COL"));
	//if (RightCollision)
	//	RightCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginRight);
	//else UE_LOG(LogTemp, Warning, TEXT("NO RIGHT COL"));

	CollisionArray.Add(UpCollision);
	CollisionArray.Add(DownCollision);
	CollisionArray.Add(LeftCollision);
	CollisionArray.Add(RightCollision);

	//UE_LOG(LogTemp, Warning, TEXT("LEFT Collision: %s"), *LeftCollision->GetComponentLocation().ToString());

}

void AArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//bGenerateOverlapEventsDuringLevelStreaming = true;
	UpdateOverlaps(true);

	if (DownCollision)
		DownCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginBottom);
	if (UpCollision)
		UpCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginUp);
	if (LeftCollision)
		LeftCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginLeft);
	if (RightCollision)
		RightCollision->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginRight);

	//Location = GetActorLocation();
}

bool AArea::Split()
{
	SplitH = (bool)FMath::RandRange(0, 1);
	if (Width > Height&& Width / Height >= 1.25)
		SplitH = false;
	else if (Height > Width&& Height / Width >= 1.25)
		SplitH = true;
	Max = (SplitH ? Height : Width) - MIN_AREA_SIZE;

	if (Max > MIN_AREA_SIZE)
	{
		return true;
	}
	return false;
}


void AArea::SpawnChild()
{
	if (!Visited)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		float Split = FMath::RandRange(MIN_AREA_SIZE, Max);
		if (SplitH)
		{
			RightChildTransform = FTransform(
				FRotator::ZeroRotator,
				FVector(Location.X, Location.Y + Split * 100 / 2, 0),
				FVector(Width, Height - Split, 1));

			RightAreaChild = Cast<AArea>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_Area, RightChildTransform));
			if (RightAreaChild)
			{
				Location = GetActorLocation();
				RightAreaChild->Width = Width;
				RightAreaChild->Height = Height - Split;
				UGameplayStatics::FinishSpawningActor(RightAreaChild, RightChildTransform);
			}
			RightAreaChild->Parent = this;

			LeftChildTransform = FTransform(
				FRotator::ZeroRotator,
				FVector(Location.X, RightAreaChild->Location.Y - RightAreaChild->Height * 100 / 2 - Split * 100 / 2, 0),
				FVector(Width, Split, 1));
			LeftAreaChild = Cast<AArea>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_Area, LeftChildTransform));
			if (LeftAreaChild)
			{
				Location = GetActorLocation();
				LeftAreaChild->Width = Width;
				LeftAreaChild->Height = Split;
				UGameplayStatics::FinishSpawningActor(LeftAreaChild, LeftChildTransform);
			}
			LeftAreaChild->Parent = this;
		}
		else
		{
			RightChildTransform = FTransform(
				FRotator::ZeroRotator,
				FVector(Location.X + Split * 100 / 2, Location.Y, 0),
				FVector(Width - Split, Height, 1));

			RightAreaChild = Cast<AArea>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_Area, RightChildTransform));//(BP_Area, AArea::StaticClass(), RightChildTransform));
			if (RightAreaChild)
			{
				Location = GetActorLocation();
				RightAreaChild->Width = Width - Split;
				RightAreaChild->Height = Height;
				UGameplayStatics::FinishSpawningActor(RightAreaChild, RightChildTransform);
			}
			RightAreaChild->Parent = this;

			LeftChildTransform = FTransform(
				FRotator::ZeroRotator,
				FVector(RightAreaChild->Location.X - RightAreaChild->Width * 100 / 2 - Split * 100 / 2, Location.Y, 0),
				FVector(Split, Height, 1));

			LeftAreaChild = Cast<AArea>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_Area, LeftChildTransform));
			if (LeftAreaChild)
			{
				Location = GetActorLocation();
				LeftAreaChild->Width = Split;
				LeftAreaChild->Height = Height;
				UGameplayStatics::FinishSpawningActor(LeftAreaChild, LeftChildTransform);
			}
			LeftAreaChild->Parent = this;
		}
		//Box->DestroyComponent();

		DestroyCollisions();
		Childs.Add(LeftAreaChild);
		Childs.Add(RightAreaChild);
		Visited = true;

		Destroy();
	}
}

// Called every frame
void AArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AArea::SetGameMode()
{
	GameMode = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
}

bool AArea::HasChildren()
{
	return Childs.Num() > 0;
}

void AArea::CreateRooms()
{
	if (HasChildren())
	{
		LeftAreaChild->CreateRooms();
		RightAreaChild->CreateRooms();
	}
	else CreateRoom();
}

void AArea::CreateRoom()
{
	int RoomWidth = 5;
	int RoomHeight = 5;
	for (int i = Width - FMath::RandRange(5, 9); i > 5; i--)
	{
		//CHECK WHAT HAPPENS
		//if (i % 5 == 0)
		{
			RoomWidth = i;
			break;
		}
	}
	for (int i = Height - FMath::RandRange(5, 9); i > 5; i--)
	{
		//if (i % 5 == 0)
		{
			RoomHeight = i;
			break;
		}
	}

	FTransform RoomTransform = FTransform(
		FRotator::ZeroRotator,
		Location,
		FVector(RoomWidth, RoomHeight, 1));

	Room = Cast<ARoom>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AreaRoom, RoomTransform));//GetWorld()->SpawnActor<ARoom>(AreaRoom, RoomTransform.GetLocation(),FRotator::ZeroRotator);
	if (Room)
	{
		Room->Width = RoomWidth;
		Room->Height = RoomHeight;
		Room->Location = Location;
		UGameplayStatics::FinishSpawningActor(Room, RoomTransform);
	}
	Room->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
}

void AArea::CreateExit(int X, int Y, bool IsRight)
{
	if (IsRight)
	{
		AArea* Hall = GetWorld()->SpawnActor<AArea>(BP_MAIN_Hall);
		Hall->SetActorScale3D(FVector(7, 4, 1));
		Hall->SetActorLocation(FVector(X + Hall->GetActorScale3D().X * 100 / 2, Y, 1));
		Hall->Location = Hall->GetActorLocation();
		Hall->Height = Hall->GetActorScale3D().Y;
		Hall->Width = Hall->GetActorScale3D().X;

		Room->RightExit = Hall;
		Room->CreatedExits.Add(Hall);
		GameMode->IsRightExitSet = true;

		auto LeftWall = GetWorld()->SpawnActor<AWall>(BP_Wall);
		LeftWall->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y + Hall->Height * 100 / 2, 1000));
		LeftWall->SetActorScale3D(FVector(Hall->Width, 1, 20));
		LeftWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		LeftWall->Wall->SetMaterial(0, HallWallMaterial);
		//UE_LOG(LogTemp,Warning, TEXT("MATERIAL: %s, Wall: %s"), *LeftWall->Wall->GetMaterial(0)->GetName(), *LeftWall->GetName());

		auto RightWall = GetWorld()->SpawnActor<AWall>(BP_Wall);
		RightWall->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y - Hall->Height * 100 / 2, 1000));
		RightWall->SetActorScale3D(FVector(Hall->Width, 1, 20));
		RightWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		RightWall->Wall->SetMaterial(0, HallWallMaterial);

		auto DownExit = GetWorld()->SpawnActor<AExit>(BP_Exit);
		DownExit->SetActorLocation(FVector(Hall->Location.X - Hall->Width * 100 / 2, Hall->Location.Y, 50));
		DownExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");

		auto UpExit = GetWorld()->SpawnActor<AArea>(BP_Hall);
		UpExit->SetActorLocation(FVector(Hall->Location.X + Hall->Width * 100 / 2, Hall->Location.Y, 1000));
		UpExit->SetActorRotation(FRotator(0, 180, 0));
		UpExit->SetActorScale3D(FVector(1, Hall->Height, 20));
		UpExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		GameMode->MainRightExit = Hall;
	}
	else
	{
		AArea* Hall = GetWorld()->SpawnActor<AArea>(BP_MAIN_Hall);
		Hall->SetActorScale3D(FVector(4, 7, 1));
		Hall->SetActorLocation(FVector(X, Y + Hall->GetActorScale3D().Y * 100 / 2, 1));
		Hall->Location = Hall->GetActorLocation();
		Hall->Height = Hall->GetActorScale3D().Y;
		Hall->Width = Hall->GetActorScale3D().X;

		Room->UpExit = Hall;
		Room->CreatedExits.Add(Hall);
		GameMode->IsUpExitSet = true;

		auto LeftWall = GetWorld()->SpawnActor<AWall>(BP_Wall);
		LeftWall->SetActorLocation(FVector(Hall->Location.X + Hall->Width * 100 / 2, Hall->Location.Y, 1000));
		LeftWall->SetActorScale3D(FVector(1, Hall->Height, 20));
		LeftWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		LeftWall->Wall->SetMaterial(0, HallWallMaterial);

		auto RightWall = GetWorld()->SpawnActor<AWall>(BP_Wall);
		RightWall->SetActorLocation(FVector(Hall->Location.X - Hall->Width * 100 / 2, Hall->Location.Y, 1000));
		RightWall->SetActorScale3D(FVector(1, Hall->Height, 20));
		RightWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		RightWall->Wall->SetMaterial(0, HallWallMaterial);

		auto DownExit = GetWorld()->SpawnActor<AArea>(BP_Hall);
		DownExit->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y + Hall->Height * 100 / 2, 1000));
		DownExit->SetActorScale3D(FVector(Hall->Width, 1, 20));
		DownExit->SetActorRotation(FRotator(0));
		DownExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");

		auto UpExit = GetWorld()->SpawnActor<AExit>(BP_Exit);
		UpExit->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y - Hall->Height * 100 / 2, 50));
		UpExit->SetActorRotation(FRotator(0, 90, 0));
		UpExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		GameMode->MainUpExit = Hall;
	}
}

void AArea::CreateInterior()
{
	Room->CreateProps();
	Room->CreateWalls();
	Room->CreateDecorWalls();
}

ARoom* AArea::GetRoom()
{
	if (Room)
		return Room;
	else
	{
		if (LeftAreaChild != nullptr)
			return LeftAreaChild->GetRoom();

		if (RightAreaChild != nullptr)
			return RightAreaChild->GetRoom();

		if (RightAreaChild == nullptr && LeftAreaChild == nullptr)
			return nullptr;
	}
	return nullptr;
}

void AArea::CreateHall()
{
	//UE_LOG(LogTemp, Warning, TEXT(" ROOM: %s "), *GetRoom()->GetName());
	constexpr int ROOT_AREA_SCALE = 15000;
	int AdditionalOffset = 150;
	/*if (!RightRoom && Room && Room->Location.X > ROOT_AREA_SCALE - 2000)
	{
		if (!GameMode->IsRightExitSet)
			CreateExit(Room->Location.X + Room->Width * 100 / 2, Room->Location.Y, true);
	}
	if (!UpRoom && Room && Room->Location.Y > ROOT_AREA_SCALE - 2000)
	{
		if (!GameMode->IsUpExitSet)
			CreateExit(Room->Location.X, Room->Location.Y + Room->Height * 100 / 2, false);
	}*/
	//auto UpColLoc = UpCollision->GetComponentLocation();
	if (UpCollision && UpRoom && (UpRoom->Width / Room->Width >= 2 || UpRoom->Width / Room->Width <= 1) &&
		!UpRoom->DownExit &&
		UpCollision->GetComponentLocation().X < UpRoom->Location.X + UpRoom->Width * 100 / 2 - AdditionalOffset &&
		UpCollision->GetComponentLocation().X > UpRoom->Location.X - UpRoom->Width * 100 / 2 + AdditionalOffset)
	{
		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(UpRoom->Location.X, UpRoom->Location.Y, 0);
		float HallWidth = Point2.X - Point1.X;
		float HallHeight = Point2.Y - UpRoom->Height * 100 / 2 - Point1.Y + Room->Height * 100 / 2;
		float Height = FVector::Distance(FVector(0, UpRoom->Location.Y - UpRoom->Height * 100 / 2, 0), FVector(0, Room->Location.Y + Room->Height * 100 / 2, 0));
		float Y = (UpRoom->Location.Y - UpRoom->Height * 100 / 2) + (Room->Location.Y + Room->Height * 100 / 2);

		auto Hall = SpawnHall(Room->Location.X, Y / 2, 4, Height / 100);;
		Hall->Childs.Add(Room);
		Hall->Childs.Add(UpRoom);
		Room->UpExit = Hall;
		Room->CreatedExits.Add(Hall);
		UpRoom->DownExit = Hall;
		UpRoom->CreatedExits.Add(Hall);
	}

	//auto DownColLoc = DownCollision->GetComponentLocation();
	if (DownCollision && DownRoom && (DownRoom->Width / Room->Width >= 2 || DownRoom->Width / Room->Width <= 1) &&
		!DownRoom->UpExit &&
		DownCollision->GetComponentLocation().X < DownRoom->Location.X + DownRoom->Width * 100 / 2 - AdditionalOffset &&
		DownCollision->GetComponentLocation().X > DownRoom->Location.X - DownRoom->Width * 100 / 2 + AdditionalOffset)
	{
		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(DownRoom->Location.X, DownRoom->Location.Y, 0);
		float HallWidth = Point2.X - Point1.X;
		float HallHeight = Point2.Y + DownRoom->Height * 100 / 2 - Point1.Y - Room->Height * 100 / 2;
		float Height = FVector::Distance(FVector(0, DownRoom->Location.Y + DownRoom->Height * 100 / 2, 0), FVector(0, Room->Location.Y - Room->Height * 100 / 2, 0));
		float Y = (DownRoom->Location.Y + DownRoom->Height * 100 / 2) + (Room->Location.Y - Room->Height * 100 / 2);

		auto Hall = SpawnHall(Room->Location.X, Y / 2, 4, Height / 100);;
		Hall->Childs.Add(Room);
		Hall->Childs.Add(DownRoom);
		Room->DownExit = Hall;
		Room->CreatedExits.Add(Hall);
		DownRoom->UpExit = Hall;
		DownRoom->CreatedExits.Add(Hall);
		//UE_LOG(LogTemp, Warning, TEXT("CREATE HALL"));
	}
	//auto RightColLoc = RightCollision->GetComponentLocation();
	if (RightCollision && RightRoom && (RightRoom->Height / Room->Height >= 2 || RightRoom->Height / Room->Height <= 1) &&
		!RightRoom->LeftExit &&
		RightCollision->GetComponentLocation().Y < RightRoom->Location.Y + RightRoom->Height * 100 / 2 - AdditionalOffset &&
		RightCollision->GetComponentLocation().Y > RightRoom->Location.Y - RightRoom->Height * 100 / 2 + AdditionalOffset
		)
	{
		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(RightRoom->Location.X, RightRoom->Location.Y, 0);
		float HallWidth = Point2.X - RightRoom->Width * 100 / 2 - Point1.X + Room->Width * 100 / 2;
		float HallHeight = Point2.Y - Point1.Y;
		float Width = FVector::Distance(FVector(RightRoom->Location.X - RightRoom->Width * 100 / 2, 0, 0), FVector(Room->Location.X + Room->Width * 100 / 2, 0, 0));
		float X = (RightRoom->Location.X - RightRoom->Width * 100 / 2) + (Room->Location.X + Room->Width * 100 / 2);

		auto Hall = SpawnHall(X / 2, Room->Location.Y, Width / 100, 4);
		Hall->Childs.Add(Room);
		Hall->Childs.Add(RightRoom);
		Room->RightExit = Hall;
		Room->CreatedExits.Add(Hall);
		RightRoom->LeftExit = Hall;
		RightRoom->CreatedExits.Add(Hall);
	}
	//auto LeftColLoc = LeftCollision->GetComponentLocation();
	if (LeftCollision && LeftRoom && (LeftRoom->Height / Room->Height >= 2 || LeftRoom->Height / Room->Height <= 1) &&
		!LeftRoom->RightExit &&
		LeftCollision->GetComponentLocation().Y < LeftRoom->Location.Y + LeftRoom->Height * 100 / 2 - AdditionalOffset &&
		LeftCollision->GetComponentLocation().Y > LeftRoom->Location.Y - LeftRoom->Height * 100 / 2 + AdditionalOffset)
	{
		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(LeftRoom->Location.X, LeftRoom->Location.Y, 0);
		float HallWidth = Point2.X + LeftRoom->Width * 100 / 2 - Point1.X - Room->Width * 100 / 2;
		float HallHeight = Point2.Y - Point1.Y;
		float Width = FVector::Distance(FVector(LeftRoom->Location.X + LeftRoom->Width * 100 / 2, 0, 0), FVector(Room->Location.X - Room->Width * 100 / 2, 0, 0));
		float X = (LeftRoom->Location.X + LeftRoom->Width * 100 / 2) + (Room->Location.X - Room->Width * 100 / 2);

		auto Hall = SpawnHall(X / 2, Room->Location.Y, Width / 100, 4);
		Hall->Childs.Add(Room);
		Hall->Childs.Add(LeftRoom);
		Room->LeftExit = Hall;
		Room->CreatedExits.Add(Hall);
		LeftRoom->RightExit = Hall;
		LeftRoom->CreatedExits.Add(Hall);
	}
}

AArea* AArea::SpawnHall(int X, int Y, float Width, float Height)
{
	int WallOffset = 0;//50;
	FTransform HallTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(X, Y, 0),
		FVector(Width, Height, 1));

	AArea* Hall = Cast<AArea>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_Hall, HallTransform));
	if (Hall)
	{
		Location = GetActorLocation();
		Hall->Width = Width;
		Hall->Height = Height;
		UGameplayStatics::FinishSpawningActor(Hall, HallTransform);
	}

	if (Width > Height)
	{
		auto LeftWall = GetWorld()->SpawnActor<AWall>/*AArea>*/(BP_Wall);//BP_Hall);
		LeftWall->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y + Hall->Height * 100 / 2 + WallOffset, 1000));
		LeftWall->SetActorScale3D(FVector(Width, 1, 20));
		LeftWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		LeftWall->Wall->SetMaterial(0, HallWallMaterial);

		auto RightWall = GetWorld()->SpawnActor<AWall>/*AArea>*/(BP_Wall);//BP_Hall);
		RightWall->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y - Hall->Height * 100 / 2 - WallOffset, 1000));
		RightWall->SetActorScale3D(FVector(Width, 1, 20));
		RightWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		RightWall->Wall->SetMaterial(0, HallWallMaterial);

		auto DownExit = GetWorld()->SpawnActor<AExit>(BP_Exit);
		DownExit->SetActorLocation(FVector(Hall->Location.X - Hall->Width * 100 / 2, Hall->Location.Y, 50));
		//DownExit->SetActorRotation(FRotator(0,180,0));
		DownExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");

		auto UpExit = GetWorld()->SpawnActor<AExit>(BP_Exit);
		UpExit->SetActorLocation(FVector(Hall->Location.X + Hall->Width * 100 / 2, Hall->Location.Y, 50));
		UpExit->SetActorRotation(FRotator(0, 180, 0));
		UpExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
	}
	else if (Width < Height)
	{
		auto LeftWall = GetWorld()->SpawnActor<AWall>/*AArea>*/(BP_Wall);//BP_Hall);
		LeftWall->SetActorLocation(FVector(Hall->Location.X + Hall->Width * 100 / 2 + WallOffset, Hall->Location.Y, 1000));
		LeftWall->SetActorScale3D(FVector(1, Height, 20));
		LeftWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		LeftWall->Wall->SetMaterial(0, HallWallMaterial);

		auto RightWall = GetWorld()->SpawnActor<AWall>/*AArea>*/(BP_Wall);//BP_Hall);
		RightWall->SetActorLocation(FVector(Hall->Location.X - Hall->Width * 100 / 2 - WallOffset, Hall->Location.Y, 1000));
		RightWall->SetActorScale3D(FVector(1, Height, 20));
		RightWall->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
		RightWall->Wall->SetMaterial(0, HallWallMaterial);

		auto DownExit = GetWorld()->SpawnActor<AExit>(BP_Exit);
		DownExit->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y + Hall->Height * 100 / 2, 50));
		//DownExit->SetActorRotation(FRotator(0,90,0));
		DownExit->SetActorRotation(FRotator(0, 270, 0));
		DownExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");

		auto UpExit = GetWorld()->SpawnActor<AExit>(BP_Exit);
		UpExit->SetActorLocation(FVector(Hall->Location.X, Hall->Location.Y - Hall->Height * 100 / 2, 50));
		UpExit->SetActorRotation(FRotator(0, 90, 0));
		UpExit->AttachToActor(Hall, FAttachmentTransformRules::KeepWorldTransform, "");
	}
	Hall->Parent = this;
	return Hall;
}

void AArea::DestroyCollisions()
{
	for (auto Collision : CollisionArray)
	{
		Collision->DestroyComponent();
	}
}



