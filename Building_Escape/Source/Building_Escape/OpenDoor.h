// Copyright Craig Palmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

private:
	
	float initial{ 0 }, target{ 0 }, current{ 0 };
	float doorLastOpened{ 0.f };
	bool bDoorOpen{ false };
	UPROPERTY()
	UAudioComponent* audioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float doorCloseDelay{ 0.5f };
	UPROPERTY(EditAnywhere)
	float doorOpenSpeed{ 50.f };
	UPROPERTY(EditAnywhere)
	float doorCloseSpeed{ 150.f };
	UPROPERTY(EditAnywhere)
	float amountToRotate{ 90 };
	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate {nullptr};
	UPROPERTY(EditAnywhere)
	float massToOpenDoor{ 10.f };

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(const float& DeltaTime);
	void CloseDoor(const float& DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();
};
