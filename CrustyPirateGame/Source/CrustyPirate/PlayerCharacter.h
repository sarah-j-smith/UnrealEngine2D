// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Engine/TimerHandle.h"
#include "Sound/SoundBase.h"

#include "PaperZDAnimInstance.h"
#include "CrustyPirateGameInstance.h"
#include "PlayerHUD.h"

#include "CollectableItem.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API APlayerCharacter : public APaperZDCharacter
{
    GENERATED_BODY()
public:
    
    // MARK: Components
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USpringArmComponent *springArm;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCameraComponent *camera;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent *attackCollisionBox;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UTextRenderComponent *HPText;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UPlayerHUD> PlayerHUDClass;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPlayerHUD *PlayerHUDWidget;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCrustyPirateGameInstance *MyGameInstance;
    
    // MARK: Inputs
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputMappingContext *inputMappingContext;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction *attackAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction *jumpAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction *moveAction;

    // MARK: Animations
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence *attackAnimSequence;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase *ItemPickupSound;
    
    // MARK: Damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int hitPoints = 100;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int AttackDamage = 25;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackStunDuration = 0.3f;
    
    // MARK: Control Vars
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float moveSpeed;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool isAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool isStunned = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool canMove = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool canAttack = true;
    
    FTimerHandle stunTimer;
    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
    
    APlayerCharacter();
    
    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
    
    
    void Stun(float DurationInSeconds);
    
    void OnStunTimerTimeout();
    
    void Attack(const FInputActionValue &Value);
    
    void JumpStarted(const FInputActionValue &Value);
    
    void JumpEnded(const FInputActionValue &Value);
    
    void Move(const FInputActionValue &Value);
    
    void UpdateDirection(float moveDirection);
    
    void OnAttackOverrideAnimEnd(bool completed);
    
    UFUNCTION()
    void AttackBoxOverlapBegin(UPrimitiveComponent *OverlappedComponent,
                               AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                               int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);
    
    UFUNCTION(BlueprintCallable)
    void EnableAttackCollisionBox(bool enabled);
    
    void UpdateHitPoints(int NewHitPoints);
    
    void ApplyDamage(int DamageAmount, float StunDuration);
    
    void CollectItem(CollectableType ItemType);
};
