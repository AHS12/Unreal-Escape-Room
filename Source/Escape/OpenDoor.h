// Copyright @AHS12 2021 All Right Preserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActorsOnPlate() const;

private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	float DoorLastOpenedTime = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;	

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 75.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 130.f;
	UPROPERTY(EditAnywhere)
	float TriggerMass = 100.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;


		
};
