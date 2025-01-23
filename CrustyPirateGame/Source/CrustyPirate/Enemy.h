// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TimerHandle.h"
#include "PaperZDAnimInstance.h"

#include "PlayerCharacter.h"

#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API AEnemy : public APaperZDCharacter
{
    GENERATED_BODY()
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UTextRenderComponent *HPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent *playerDetectorSphere;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerCharacter *followTarget;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence *attackAnimSequence;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float stopDistanceToTarget;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool canMove = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool isAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool isStunned = false;
    
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool canAttack = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int hitPoints = 100;
    
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float attackCooldownInSeconds = 3.0f;
    
    FTimerHandle stunTimer;
    FTimerHandle attackCooldownTimer;
    
    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
    
    AEnemy();
    
    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION()
    void DetectorOverlapBegin(
                              UPrimitiveComponent *OverlappedComponent,
                              AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                              int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);
    
    UFUNCTION()
    void DetectorOverlapEnd(
                            UPrimitiveComponent *OverlappedComponent,
                            AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                            int32 OtherBodyIndex);
    
    bool ShouldMoveToTarget();
    
    void UpdateDirection(float moveDirection);
    
    void UpdateHitPoints(int NewHitPoints);
    
    void ApplyDamage(int DamageAmount, float StunDuration);
    
    void Stun(float DurationInSeconds);
    
    void OnStunTimerTimeout();
    
    void Attack();
    
    void OnAttackCooldownTimerTimeout();
    
    void OnAttackOverrideAnimEnd(bool completed);
};
