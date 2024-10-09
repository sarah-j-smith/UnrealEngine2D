// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API APlayerCharacter : public APaperZDCharacter
{
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USpringArmComponent *springArm;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCameraComponent *camera;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputMappingContext *inputMappingContext;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction *attackAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction *jumpAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction *moveAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float moveSpeed;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool isAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool canMove = true;
    
    APlayerCharacter();
    
    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
    
    void Attack(const FInputActionValue &Value);
    
    void JumpStarted(const FInputActionValue &Value);
    
    void JumpEnded(const FInputActionValue &Value);
    
    void Move(const FInputActionValue &Value);
    
    void UpdateDirection(float moveDirection);
};
