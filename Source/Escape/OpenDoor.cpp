// Copyright @AHS12 2021 All Right Preserved

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	//logging
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("OpenDoor Tick : %s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Current %s Rotation: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);

	// ...
	if (PressurePlate && TotalMassOfActorsOnPlate() > TriggerMass)
	{
		/* code */
		OpenDoor(DeltaTime);
		DoorLastOpenedTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		CloseDoor(DeltaTime);
		// if (GetWorld()->GetTimeSeconds() - DoorLastOpenedTime > DoorCloseDelay)
		// {
		// 	//CloseDoor(DeltaTime);
		// }
		//CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	FRotator OpenDoorRotation = GetOwner()->GetActorRotation();
	OpenDoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoorRotation);
}
void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator OpenDoorRotation = GetOwner()->GetActorRotation();
	OpenDoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoorRotation);
}

float UOpenDoor::TotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.f;

	TArray<AActor *> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (const auto &Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate has a total mass of %f"), *GetOwner()->GetName(), TotalMass);
	return TotalMass;
}
