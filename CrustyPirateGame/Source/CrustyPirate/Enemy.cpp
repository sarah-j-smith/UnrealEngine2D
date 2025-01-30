// (c) 2024 Sarah Smith


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy() {
    PrimaryActorTick.bCanEverTick = true;
    
    playerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
    playerDetectorSphere->SetupAttachment(RootComponent);
    
    HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
    HPText->SetupAttachment(RootComponent);
    
    attackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision Box"));
    attackCollisionBox->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    
    playerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
    playerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
    
    UpdateHitPoints(hitPoints);
    
    OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);
    
    attackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
    
    EnableAttackCollisionBox(false);
}

void AEnemy::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    
    if (isAlive && followTarget && !isStunned) {
        float moveDirection = (followTarget->GetActorLocation().X - GetActorLocation().X) > 0.0 ? 1.0f : -1.0f;
        UpdateDirection(moveDirection);
        if (ShouldMoveToTarget()) {
            if (canMove) {
                FVector worldDirection = FVector(1.0f, 0.0f, 0.0f);
                AddMovementInput(worldDirection, moveDirection);
            }
        } else {
            if (followTarget->isAlive)
            {
                Attack();
            }
        }
    }
}

bool AEnemy::ShouldMoveToTarget() {
    bool result = false;
    
    if (followTarget) {
        float distance = abs(followTarget->GetActorLocation().X - GetActorLocation().X);
        result = distance > stopDistanceToTarget;
    }
    
    return result;
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent *OverlappedComponent,
                                  AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                                  int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {
    APlayerCharacter *playerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (playerCharacter) {
        followTarget = playerCharacter;
    }
}

void AEnemy::DetectorOverlapEnd(UPrimitiveComponent *OverlappedComponent,
                                AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                                int32 OtherBodyIndex)
{
    APlayerCharacter *playerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (playerCharacter) {
        followTarget = nullptr;
    }
}

void AEnemy::UpdateDirection(float moveDirection) {
    FRotator currentRotation = GetActorRotation();
    
    if (moveDirection < 0.0f) {
        if (currentRotation.Yaw != 180.0f) {
            SetActorRotation(FRotator(currentRotation.Pitch, 180.0f, currentRotation.Roll));
        }
    } else if (moveDirection > 0.0f) {
        if (currentRotation.Yaw != 0.0f) {
            SetActorRotation(FRotator(currentRotation.Pitch, 0.0f, currentRotation.Roll));
        }
    }
}

void AEnemy::UpdateHitPoints(int NewHitPoints)
{
    hitPoints = NewHitPoints;
    
    FString hpStr = FString::Printf(TEXT("HP: %d"), hitPoints);
    HPText->SetText(FText::FromString(hpStr));
}

void AEnemy::ApplyDamage(int DamageAmount, float StunDuration)
{
    if (!isAlive) return;
    
    UpdateHitPoints(hitPoints - DamageAmount);
    
    if (hitPoints <= 0) {
        // Is Dead
        
        UpdateHitPoints(0);
        HPText->SetHiddenInGame(true);
        
        isAlive = false;
        canMove = false;
        canAttack = false;
        
        // Play the die animation
        GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CrabbyStateMachine"));
        
        EnableAttackCollisionBox(false);
    } else {
        // Is Alive
        
        // Play the take damage animation
        GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("CrabbyStateMachine"));
        
        Stun(StunDuration);
    }
}

void AEnemy::Stun(float DurationInSeconds) {
    GetWorldTimerManager().SetTimer(
        stunTimer, this,
        &AEnemy::OnStunTimerTimeout, 1.0, false, DurationInSeconds);
    
    GetAnimInstance()->StopAllAnimationOverrides();
    EnableAttackCollisionBox(false);
}

void AEnemy::OnStunTimerTimeout() {
    isStunned = false;
}

void AEnemy::Attack()
{
    if (isAlive && !isStunned && canAttack)
    {
        canAttack = false;
        canMove = false;
        
        UPaperZDAnimInstance *anim = GetAnimInstance();
        anim->PlayAnimationOverride(attackAnimSequence, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
        
        // Set cooldown timer
        GetWorldTimerManager().SetTimer(
            attackCooldownTimer, this,
            &AEnemy::OnAttackCooldownTimerTimeout, 1.0,
            false, attackCooldownInSeconds);
    }
}

void AEnemy::OnAttackCooldownTimerTimeout()
{
    if (isAlive)
    {
        canAttack = true;
    }
}

void AEnemy::OnAttackOverrideAnimEnd(bool completed)
{
    if (isAlive)
    {
        canMove = true;
    }
}

void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent *OverlappedComponent,
                           AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                           int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult)
{
    APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
    if (player) {
        // take damage
        player->ApplyDamage(AttackDamage, AttackStunDuration);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Player take damage"));
    }
}

void AEnemy::EnableAttackCollisionBox(bool enabled)
{
    if (enabled)
    {
        attackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        attackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    }
    else
    {
        attackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        attackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    }
}
