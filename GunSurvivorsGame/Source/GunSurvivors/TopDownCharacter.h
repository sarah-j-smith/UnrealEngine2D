// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Engine/TimerHandle.h"

#include "Bullet.h"

#include "TopDownCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);

UCLASS()
class GUNSURVIVORS_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

	void Die();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent *capsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent *characterFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent *gunParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent *gunSprite;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent *bulletSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext *inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction *moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction *shootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook *idleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook *runFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase *hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase *bulletSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D movementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool canMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapLimitsHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapLimitsVertical;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActorToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool canShoot = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isAlive = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool hasMouse = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool hasTouch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float shootCooldownDurationInSeconds = 0.3f;

	FTimerHandle ShootCooldownTimer;

	FPlayerDiedDelegate playerDiedDelegate;

	ATopDownCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue &);

	void MoveCompleted(const FInputActionValue &);

	void Shoot(const FInputActionValue &);

	bool IsInMapBoundsHorizontal(float xpos);

	bool IsInMapBoundsVertical(float zpos);

	void OnShootCooldownTimerTimeout();

	UFUNCTION()
	void OverlapBegin(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor, UPrimitiveComponent *OtherComponent,
		int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);
};
