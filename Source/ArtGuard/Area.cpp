// Fill out your copyright notice in the Description page of Project Settings.


#include "Area.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ArtGuardGameMode.h"
#include "Room.h"
#include "Kismet/GameplayStatics.h"
//#include "Components/SphereComponent.h"
#include "Exit.h"
#include "Space.h"
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

	HallsNumber = 0;

	Visited = false;

	LeftAreaChild = nullptr;
	RightAreaChild = nullptr;

	FTransform BoxTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0),
		FVector(1, 1, 1)
	);

	Box = CreateDefaultSubobject<UStaticMeshComponent>(FName("Box"));
	Box->SetWorldTransform(BoxTransform);
	SetRootComponent(Box);
	UpCollision = CreateDefaultSubobject<UBoxComponent>(FName("UpCollision")); //<UBoxComponent>(this, TEXT("UpCollision"));
	DownCollision = CreateDefaultSubobject<UBoxComponent>(FName("DownCollision")); //ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("DownCollision"));
	RightCollision = CreateDefaultSubobject<UBoxComponent>(FName("RightCollision")); //ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("RightCollision"));
	LeftCollision = CreateDefaultSubobject<UBoxComponent>(FName("LeftCollision")); //ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("LeftCollision"));


	FTransform UpTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0), //y=105
		FVector(0.5, 1, 1) //y=0.5
	);
	FTransform DownTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0),//y=-105
		FVector(0.5, 1, 1) //y=0.5
	);

	FTransform RightTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0), //x= 105
		FVector(1, 0.5, 1) //x=0.5
	);

	FTransform LeftTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0), //x=-105
		FVector(1, 0.5, 1) //x=0.5
	);


	RoomDetectionBox = CreateDefaultSubobject<UBoxComponent>(FName("DetectionSphere"));
	FTransform SphereTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0), //x=-105
		FVector(1)//(FMath::Sqrt((Width * Width) + (Height * Height)) / 2) //x=0.5
	);
	//float SphereRadius = FMath::Sqrt((Width * Width) + (Height * Height)) / 2;

	RoomDetectionBox->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	RoomDetectionBox->SetGenerateOverlapEvents(true);

	UpCollision->SetWorldTransform(UpTransform);
	DownCollision->SetWorldTransform(DownTransform);
	RightCollision->SetWorldTransform(RightTransform);
	LeftCollision->SetWorldTransform(LeftTransform);

	UpCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	DownCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	RightCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
	LeftCollision->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);

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
	if (OtherActor->ActorHasTag("Room") && OtherActor != Room)//this)
	{
		UpRoom = Cast<ARoom>(OtherActor);
		UP = OtherActor->GetName();
	}
}

void AArea::OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != Room)//this)
	{
		DownRoom = Cast<ARoom>(OtherActor);
		DOWN = OtherActor->GetName();
	}
}

void AArea::OnOverlapBeginRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != Room)//this)
	{
		RightRoom = Cast<ARoom>(OtherActor);
		RIGHT = OtherActor->GetName();
	}
}

void AArea::OnOverlapBeginLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != Room)//this)
	{
		LeftRoom = Cast<ARoom>(OtherActor);
		LEFT = OtherActor->GetName();
	}
}

void AArea::OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != Room)//this)
	{
		//if (FVector::Distance(Room->GetActorLocation(), OtherActor->GetActorLocation()) < RoomDetectionSphere->GetScaledSphereRadius() * 2)
		NeighbourRooms.Add(OtherActor);
	}
}

void AArea::OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != Room)//this)
	{
		//if (FVector::Distance(Room->GetActorLocation(), OtherActor->GetActorLocation()) < RoomDetectionSphere->GetScaledSphereRadius() * 2)
		NeighbourRooms.Remove(OtherActor);
	}
}


// Called when the game starts or when spawned
void AArea::BeginPlay()
{
	Super::BeginPlay();
	//bGenerateOverlapEventsDuringLevelStreaming = true;
	Location = GetActorLocation();

	CollisionArray.Add(UpCollision);
	CollisionArray.Add(DownCollision);
	CollisionArray.Add(LeftCollision);
	CollisionArray.Add(RightCollision);

	float SphereRadius = FMath::Sqrt(FMath::Pow(Width, 2) + FMath::Pow(Height, 2));
	//SphereRadius += FMath::RandRange(15, 40);//*= 100;//+= 50;
	//UE_LOG(LogTemp, Warning, TEXT("SPHERE RADIUS: %f"), SphereRadius);
	//RoomDetectionSphere->InitSphereRadius(SphereRadius);
	//RoomDetectionSphere->SetSphereRadius(SphereRadius);
	RoomDetectionBox->SetWorldScale3D(FVector(Width * 2.5, Height*2.5, 10));
	//UE_LOG(LogTemp, Warning, TEXT("SCALED RADIUS: %f"), RoomDetectionSphere->GetScaledSphereRadius());
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
	if (RoomDetectionBox) 
	{
		RoomDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AArea::OnOverlapBeginSphere);
		RoomDetectionBox->OnComponentEndOverlap.AddDynamic(this, &AArea::OnOverlapEndSphere);
	}
}

bool AArea::Split()
{
	SplitH = (bool)FMath::RandRange(0, 1);
	if (Width > Height&& Width / Height >= 1.25)
		SplitH = false;
	else if (Height > Width&& Height / Width >= 1.25)
		SplitH = true;
	Max = (SplitH ? Height : Width) - /*1.5 */ MIN_AREA_SIZE;

	if (Max > MIN_AREA_SIZE)
	{
		return true;
	}
	return false;
}


void AArea::SpawnChild()
{
	float SphereRadius = 0;
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
		Destroy();
		Box->SetHiddenInGame(true, true);
		//RoomDetectionSphere->DestroyComponent();
		DestroyCollisions();
		Childs.Add(LeftAreaChild);
		Childs.Add(RightAreaChild);
		Visited = true;

		Box->DestroyComponent();
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
	else /*if (NeighbourRooms.Num() <= 5)*/ CreateRoom();
}

void AArea::CreateRoom()
{
	if (FMath::FRandRange(0, 1) > 0.8 || NeighbourRooms.Num() < 3)// Width >= MIN_AREA_SIZE && Height >= MIN_AREA_SIZE) 
	{
		int RoomWidth = Width - FMath::RandRange(5, 9); //min=5
		int RoomHeight = Height - FMath::RandRange(5, 9);

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
			Room->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
		}

		UpCollision->SetRelativeScale3D(FVector(Room->Width, 1, 1));
		DownCollision->SetRelativeScale3D(FVector(Room->Width, 1, 1));
		RightCollision->SetRelativeScale3D(FVector(1, Room->Height, 1));
		LeftCollision->SetRelativeScale3D(FVector(1, Room->Height, 1));

		RoomDetectionBox->SetWorldScale3D(FVector(RoomWidth * 2.5, RoomHeight * 2.5, 10));
		//float SphereRadius = FMath::Sqrt(RoomWidth * RoomWidth + RoomHeight * RoomHeight) / 2 + 50;
		//RoomDetectionSphere->SetWorldScale3D(FVector(SphereRadius));
		//RoomDetectionSphere->SetSphereRadius(SphereRadius);
	}
}

void AArea::CreateExit(int X, int Y, bool IsRight)
{
	if (IsRight)
	{
		AExit* Hall = GetWorld()->SpawnActor<AExit>(BP_MAIN_Hall);
		Hall->SetActorScale3D(FVector(7, 4, 1));
		Hall->SetActorLocation(FVector(X + Hall->GetActorScale3D().X * 100 / 2, Y, 1));
		Hall->Location = Hall->GetActorLocation();
		Hall->Height = Hall->GetActorScale3D().Y;
		Hall->Width = Hall->GetActorScale3D().X;

		//Room->RightExit = Hall;
		Room->RightExits.Add(Hall);
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
		AExit* Hall = GetWorld()->SpawnActor<AExit>(BP_MAIN_Hall);
		Hall->SetActorScale3D(FVector(4, 7, 1));
		Hall->SetActorLocation(FVector(X, Y + Hall->GetActorScale3D().Y * 100 / 2, 1));
		Hall->Location = Hall->GetActorLocation();
		Hall->Height = Hall->GetActorScale3D().Y;
		Hall->Width = Hall->GetActorScale3D().X;

		//Room->UpExit = Hall;
		Room->UpExits.Add(Hall);
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
	constexpr int ROOT_AREA_SCALE = 15000;
	int AdditionalOffset = 150;// 55;//100;//50// 100;// 150;

	if (UpCollision && UpRoom && UpRoom->DownExits.Num() == 0)
	{
		float Min, Max = 0;
		FindCommonAreasBetweenNeighbourRooms(Room, UpRoom, Min, Max, true);
		float HallLocation_X = (Min + Max) / 2;//FMath::Clamp(Room->Location.X + DeltaHall, Min + DeltaOffset, Max - DeltaOffset);
		UE_LOG(LogTemp, Warning, TEXT("Room: %s Min: %f : Max: %f"), *Room->GetName(), Min, Max);

		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(UpRoom->Location.X, UpRoom->Location.Y, 0);
		float HallHeight = Point2.Y - UpRoom->Height * 100 / 2 - Point1.Y + Room->Height * 100 / 2;
		float Height = FVector::Distance(FVector(0, UpRoom->Location.Y - UpRoom->Height * 100 / 2, 0), FVector(0, Room->Location.Y + Room->Height * 100 / 2, 0));
		float Y = (UpRoom->Location.Y - UpRoom->Height * 100 / 2) + (Room->Location.Y + Room->Height * 100 / 2);

		auto Hall = SpawnHall(HallLocation_X, Y / 2, 4, Height / 100);;
		Hall->Childs.Add(Room);
		Hall->Childs.Add(UpRoom);
		//Room->UpExit = Hall;
		Room->UpExits.Add(Hall);
		Room->CreatedExits.Add(Hall);

		//UpRoom->DownExit = Hall;
		UpRoom->DownExits.Add(Hall);
		UpRoom->CreatedExits.Add(Hall);
	}
	if (DownCollision && DownRoom && DownRoom->UpExits.Num() == 0)
	{
		float Min, Max = 0;
		FindCommonAreasBetweenNeighbourRooms(Room, DownRoom, Min, Max, true);
		float HallLocation_X = (Min + Max) / 2;//FMath::Clamp(Room->Location.X + DeltaHall, Min + DeltaOffset, Max - DeltaOffset);
		UE_LOG(LogTemp, Warning, TEXT("Room: %s Min: %f : Max: %f"), *Room->GetName(), Min, Max);

		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(DownRoom->Location.X, DownRoom->Location.Y, 0);
		float HallHeight = Point2.Y + DownRoom->Height * 100 / 2 - Point1.Y - Room->Height * 100 / 2;
		float Height = FVector::Distance(FVector(0, DownRoom->Location.Y + DownRoom->Height * 100 / 2, 0), FVector(0, Room->Location.Y - Room->Height * 100 / 2, 0));
		float Y = (DownRoom->Location.Y + DownRoom->Height * 100 / 2) + (Room->Location.Y - Room->Height * 100 / 2);

		auto Hall = SpawnHall(HallLocation_X, Y / 2, 4, Height / 100);;
		Hall->Childs.Add(Room);
		Hall->Childs.Add(DownRoom);
		//Room->DownExit = Hall;
		Room->DownExits.Add(Hall);
		Room->CreatedExits.Add(Hall);

		//DownRoom->UpExit = Hall;
		DownRoom->UpExits.Add(Hall);
		DownRoom->CreatedExits.Add(Hall);
	}
	if (RightCollision && RightRoom && RightRoom->LeftExits.Num() == 0)
	{
		float Min, Max = 0;
		FindCommonAreasBetweenNeighbourRooms(Room, RightRoom, Min, Max, false);
		float HallLocation_Y = (Min + Max) / 2;//FMath::Clamp(Room->Location.Y + DeltaHall, Min + DeltaOffset, Max - DeltaOffset);
		UE_LOG(LogTemp, Warning, TEXT("Room: %s Min: %f : Max: %f"), *Room->GetName(), Min, Max);

		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(RightRoom->Location.X, RightRoom->Location.Y, 0);
		float HallHeight = Point2.Y - Point1.Y;
		float Width = FVector::Distance(FVector(RightRoom->Location.X - RightRoom->Width * 100 / 2, 0, 0), FVector(Room->Location.X + Room->Width * 100 / 2, 0, 0));
		float X = (RightRoom->Location.X - RightRoom->Width * 100 / 2) + (Room->Location.X + Room->Width * 100 / 2);

		auto Hall = SpawnHall(X / 2, HallLocation_Y, Width / 100, 4);
		Hall->Childs.Add(Room);
		Hall->Childs.Add(RightRoom);
		//Room->RightExit = Hall;
		Room->RightExits.Add(Hall);
		Room->CreatedExits.Add(Hall);

		//RightRoom->LeftExit = Hall;
		RightRoom->LeftExits.Add(Hall);
		RightRoom->CreatedExits.Add(Hall);
	}
	if (LeftCollision && LeftRoom && LeftRoom->RightExits.Num() == 0)
	{
		float Min, Max = 0;
		FindCommonAreasBetweenNeighbourRooms(Room, LeftRoom, Min, Max, false);
		float HallLocation_Y = (Min + Max) / 2;// FMath::Clamp(Room->Location.Y + DeltaHall, Min, Max);
		UE_LOG(LogTemp, Warning, TEXT("Room: %s Min: %f : Max: %f"), *Room->GetName(), Min, Max);

		FVector Point1 = FVector(Room->Location.X, Room->Location.Y, 0);
		FVector Point2 = FVector(LeftRoom->Location.X, LeftRoom->Location.Y, 0);
		//float HallWidth = Point2.X + LeftRoom->Width * 100 / 2 - Point1.X - Room->Width * 100 / 2;
		float HallHeight = Point2.Y - Point1.Y;
		float Width = FVector::Distance(FVector(LeftRoom->Location.X + LeftRoom->Width * 100 / 2, 0, 0), FVector(Room->Location.X - Room->Width * 100 / 2, 0, 0));
		float X = (LeftRoom->Location.X + LeftRoom->Width * 100 / 2) + (Room->Location.X - Room->Width * 100 / 2);

		auto Hall = SpawnHall(X / 2, HallLocation_Y, Width / 100, 4);
		Hall->Childs.Add(Room);
		Hall->Childs.Add(LeftRoom);
		//Room->LeftExit = Hall;
		Room->LeftExits.Add(Hall);
		Room->CreatedExits.Add(Hall);

		//LeftRoom->RightExit = Hall;
		LeftRoom->RightExits.Add(Hall);
		LeftRoom->CreatedExits.Add(Hall);
	}

	if (Room->CreatedExits.Num() > 0)
	{
		if (!RightRoom && Room && Room->Location.X > ROOT_AREA_SCALE - 4000) //4000 //2000
		{
			if (!GameMode->IsRightExitSet)
				CreateExit(Room->Location.X + Room->Width * 100 / 2, Room->Location.Y, true);
		}
		if (!UpRoom && Room && Room->Location.Y > ROOT_AREA_SCALE - 4000) //4000
		{
			if (!GameMode->IsUpExitSet)
				CreateExit(Room->Location.X, Room->Location.Y + Room->Height * 100 / 2, false);
		}
	}
	else
	{
		Room->Destroy();
		Destroy(false, true);
	}
}

void AArea::DeleteExtraRooms()
{
	if (Room->CreatedExits.Num() == 0)
	{
		Room->Destroy();
		Room->DestroyRoom();
		Room = nullptr;
		Destroy(false, true);
	}
}

void AArea::FindCommonAreasBetweenNeighbourRooms(ARoom* thisRoom, ARoom* NeighbourRoom, float& Min, float& Max,
	bool isComparedByX)
{
	float MinMainRoom;
	float MaxMainRoom;

	float MinOtherRoom;
	float MaxOtherRoom;

	if (isComparedByX)
	{
		MinMainRoom = thisRoom->Location.X - (thisRoom->Width * 100 / 2);
		MaxMainRoom = thisRoom->Location.X + (thisRoom->Width * 100 / 2);

		MinOtherRoom = NeighbourRoom->Location.X - (NeighbourRoom->Width * 100 / 2);
		MaxOtherRoom = NeighbourRoom->Location.X + (NeighbourRoom->Width * 100 / 2);
	}
	else
	{
		MinMainRoom = thisRoom->Location.Y - (thisRoom->Height * 100 / 2);
		MaxMainRoom = thisRoom->Location.Y + (thisRoom->Height * 100 / 2);

		MinOtherRoom = NeighbourRoom->Location.Y - (NeighbourRoom->Height * 100 / 2);
		MaxOtherRoom = NeighbourRoom->Location.Y + (NeighbourRoom->Height * 100 / 2);
	}

	MinMainRoom > MinOtherRoom ? Min = MinMainRoom : Min = MinOtherRoom;
	MaxMainRoom < MaxOtherRoom ? Max = MaxMainRoom : Max = MaxOtherRoom;
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

	if (Width >= Height)
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



