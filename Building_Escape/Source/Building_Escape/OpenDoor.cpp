// Copyright Craig Palmer 2020


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine//World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

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
	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent() 
{
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!audioComponent)
	{
		//Error
		UE_LOG(LogTemp, Error, TEXT("%s does not have an audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	//Check if null pointer
	if (!pressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no pressure plate set in OpenDoor component"), *GetOwner()->GetName());
	}
	initial = GetOwner()->GetActorRotation().Yaw;
	current = initial;
	target = initial + amountToRotate;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (pressurePlate && TotalMassOfActors() > massToOpenDoor)
	{
		OpenDoor(DeltaTime);
		doorLastOpened = GetWorld()->GetTimeSeconds();
		if (!bDoorOpen)
		{
			audioComponent->Play();
			bDoorOpen = true;
		}
	}
	else if (GetWorld()->GetTimeSeconds() - doorLastOpened > doorCloseDelay)
	{
		CloseDoor(DeltaTime);
		if (bDoorOpen)
		{
			audioComponent->Play();
			bDoorOpen = false;
		}
	}
}


void UOpenDoor::OpenDoor(const float& DeltaTime)
{

	//Alternatives. FMath::Lerp is not machine independant so use FInterConstantTo or FInterpTo instead.
	//currentRotation.Yaw = FMath::FInterpTo(currentRotation.Yaw, targetYaw, DeltaTime, 45);
	//currentRotation.Yaw = FMath::Lerp(currentRotation.Yaw, targetYaw, 0.02);
	//FRotator currentRotation = GetOwner()->GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Door Yaw Value: %f"), GetOwner()->GetActorRotation().Yaw);
	FRotator doorRotation = GetOwner()->GetActorRotation();
	current = doorRotation.Yaw;
	current = FMath::FInterpConstantTo(current, target, DeltaTime, doorOpenSpeed);
	doorRotation.Yaw = current;
	GetOwner()->SetActorRotation(doorRotation);

}

void UOpenDoor::CloseDoor(const float& DeltaTime)
{
	FRotator doorRotation = GetOwner()->GetActorRotation();
	current = doorRotation.Yaw;
	current = FMath::FInterpConstantTo(current, initial, DeltaTime, doorCloseSpeed);
	doorRotation.Yaw = current;
	GetOwner()->SetActorRotation(doorRotation);

	
}

float UOpenDoor::TotalMassOfActors() const
{
	float totalMass{ 0.f };
	TArray<AActor*> overlappingActors;
	pressurePlate->GetOverlappingActors(OUT overlappingActors);
	for (AActor* actor : overlappingActors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return totalMass;
}


