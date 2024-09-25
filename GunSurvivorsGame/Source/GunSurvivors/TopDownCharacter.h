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

#include "TopDownCharacter.generated.h"

UCLASS()
class GUNSURVIVORS_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

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
	float movementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D movementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool canMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapLimitsHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapLimitsVertical;

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
};
