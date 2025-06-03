// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "FollowCamera.generated.h"

UCLASS()
class ADVENTUREGAME_API AFollowCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AFollowCamera();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Geometry")
	USphereComponent *FollowCameraBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	USpringArmComponent *SpringArmComponent;

	/**
	 * Box containing the limits for the camera. Camera viewport must be contained
	 * within this volume.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Geometry")
	UBoxComponent *CameraConfines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	FVector ConfinesOfCamera;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	// AAdventureCharacter *PlayerCharacter;
};
