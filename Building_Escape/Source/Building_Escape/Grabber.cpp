// Copyright Craig Palmer 2020

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT //Readablity, doesnt affect code just marks it.

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	findPhysicsHandle();
	setupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetPlayersReach());
	}
}


void UGrabber::findPhysicsHandle()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle)
	{
		//Error
		UE_LOG(LogTemp, Error, TEXT("%s has no Physics Handle Component"), *GetOwner()->GetName());
	}
}

void UGrabber::setupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		//Error
		UE_LOG(LogTemp, Error, TEXT("Input component not found!"));
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	return hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	FVector lineTraceDirection = playerViewPointRotation.Vector() * reach;
	return playerViewPointLocation + lineTraceDirection;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	//Get players viewpoint
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	return playerViewPointLocation;
}

void UGrabber::Grab()
{
	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
	if (hitResult.GetActor())
	{
		if (!physicsHandle) { return; }
		physicsHandle->GrabComponentAtLocation(componentToGrab, NAME_None, GetPlayersReach());
	}
}

void UGrabber::Released()
{
	if (!physicsHandle) { return; }
	physicsHandle->ReleaseComponent();
}


