// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "FollowCamera.generated.h"

class AAdventureCharacter;

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

	/**
	 * Camera base. This is moved to change the location of the camera.
	 * The camera should not be moved directly as that would disable any
	 * spring arm effects.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FollowCamera")
	USphereComponent *FollowCameraBase;

	/**
	 * Camera should be set to orthographic, with fixed aspect ratio
	 * of the game scene width divided by the game scene height, eg 2.21.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FollowCamera")
	UCameraComponent *CameraComponent;

	/**
	 * Camera Arm - use this to set the rotation and position of the camera,
	 * and effects like camera lag.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FollowCamera")
	USpringArmComponent *SpringArmComponent;

	/**
	 * Box containing the limits for the camera. Camera viewport must be contained
	 * within this volume.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FollowCamera")
	UBoxComponent *CameraConfines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FollowCamera")
	FVector ConfinesOfCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FollowCamera")
	float FollowSpeed = 12.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FollowCamera")
	TObjectPtr<AAdventureCharacter> PlayerCharacter;

	/// Define the CameraConfines box from the screen values entered into
	/// the blueprint for the `ConfinesOfCamera`.
	void SetupCameraConfines();

	/// Lerp the camera to a position as close as possible to the player
	/// within the confines.
	void FollowPlayer(float DeltaTime);

	/// Jump the camera straight to a position as close as possible to
	/// the player within the confines.
	void SetupCamera();

private:
	/// Initialise the control values for the box that defines the camera confines.
	void InitialiseCameraConfines();
	
	FVector ConfineMax;
	FVector ConfineMin;
};
