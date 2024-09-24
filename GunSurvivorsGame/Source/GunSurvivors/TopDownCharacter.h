// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext *inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction *moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction *shootAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D movementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool canMove = true;

	ATopDownCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue &);
	void MoveCompleted(const FInputActionValue &);
	void Shoot(const FInputActionValue &);
};
