// Copyright @AHS12 2021 All Right Preserved

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GrabberComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("GrabberComponent initiated in %s"), *GetOwner()->GetName());

	//check for physics handle component
	FindPhysicsHandleComponent();

	//setting up input component
	SetupInputComponent();
}

void UGrabberComponent::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found on %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle found on %s"), *GetOwner()->GetName());
	}
}

void UGrabberComponent::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberComponent::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberComponent::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found on %s"), *GetOwner()->GetName());
	}
}

void UGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed by %s"), *GetOwner()->GetName());

	//setting up line trace
	//FVector LineTraceEnd = GetPlayerReach();

	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	//if we hit something then attach a physics handle
	if (HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabbing %s"), *HitResult.GetActor()->GetName());
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(HitResult.GetComponent(), NAME_None, GetPlayerReach(), true);
	}
}

FVector UGrabberComponent::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabberComponent::GetPlayerWorldPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released by %s"), *GetOwner()->GetName());
	//release physics handle
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabberComponent::GetFirstPhysicsBodyInReach() const
{

	//draw a line trace
	DrawDebugLine(GetWorld(), GetPlayerWorldPosition(), GetPlayerReach(), FColor::Green, false, 0.f, 0, 5.f);

	//ray cast out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);
	return Hit;
}

// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!PhysicsHandle) { return; }
	//if physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we are holding
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}
