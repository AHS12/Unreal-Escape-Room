// Copyright @AHS12 2021 All Right Preserved
#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"
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

	
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindPressurePlate();
	FindAudioComponent();
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
		IsCloseDoorSoundPlaying = false;
		if(!AudioComponent) { return; }
		OpenDoor(DeltaTime);
		// if(!IsOpenDoorSoundPlaying) {
		// 	OpenDoor(DeltaTime);
		// 	IsOpenDoorSoundPlaying = true;
		// }
		

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

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *GetOwner()->GetName());
	}

}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	} else {
		UE_LOG(LogTemp, Warning, TEXT("%s found audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	FRotator OpenDoorRotation = GetOwner()->GetActorRotation();
	OpenDoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoorRotation);

	if (!AudioComponent) { return; }

	IsCloseDoorSoundPlaying = false;
	if(IsOpenDoorSoundPlaying == false)
	{
		AudioComponent->Play();
		IsOpenDoorSoundPlaying = true;
		//IsCloseDoorSoundPlaying = true;
	}
}
void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator OpenDoorRotation = GetOwner()->GetActorRotation();
	OpenDoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoorRotation);

	if (!AudioComponent) { return; }
	IsOpenDoorSoundPlaying = false;
	if (IsCloseDoorSoundPlaying == false)
	{
		AudioComponent->Stop();
		//IsOpenDoorSoundPlaying = true;
		IsCloseDoorSoundPlaying = true;
	}
}

float UOpenDoor::TotalMassOfActorsOnPlate() const
{


	float TotalMass = 0.f;
	if (!PressurePlate) { return TotalMass; }

	TArray<AActor *> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (const auto &Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate has a total mass of %f"), *GetOwner()->GetName(), TotalMass);
	return TotalMass;
}
