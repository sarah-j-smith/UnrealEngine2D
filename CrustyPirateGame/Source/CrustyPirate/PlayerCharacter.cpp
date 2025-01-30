// (c) 2024 Sarah Smith


#include "PlayerCharacter.h"

#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    springArm->SetupAttachment(RootComponent);
    
    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
    
    attackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision Box"));
    attackCollisionBox->SetupAttachment(RootComponent);
    
    HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Hit Point Text"));
    HPText->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    APlayerController *playerController = Cast<APlayerController>(Controller);
    if (playerController) {
        UEnhancedInputLocalPlayerSubsystem *subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
        if (subSystem) {
            subSystem->AddMappingContext(inputMappingContext, 0);
        }
    }
    
    OnAttackOverrideEndDelegate.BindUObject(this, &APlayerCharacter::OnAttackOverrideAnimEnd);
    
    attackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AttackBoxOverlapBegin);
    
    EnableAttackCollisionBox(false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    UEnhancedInputComponent *inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (inputComponent) {
        inputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        inputComponent->BindAction(attackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
        inputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStarted);
        inputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnded);
        inputComponent->BindAction(jumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::JumpEnded);
    }
}

void APlayerCharacter::Attack(const FInputActionValue &Value)
{
    // GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, TEXT("Attack"), false, FVector2D(2.0, 2.0));
    if (isAlive && canAttack) {
        canAttack = false;
        canMove = false;
        
        UPaperZDAnimInstance *anim = GetAnimInstance();
        anim->PlayAnimationOverride(attackAnimSequence, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
        
        // The AttackCollisionBox is now enabled via AnimNotifyStates
    }
}

void APlayerCharacter::JumpStarted(const FInputActionValue &Value)
{
    if (isAlive && canMove) {
        Jump();
    }
}

void APlayerCharacter::JumpEnded(const FInputActionValue &Value)
{
    StopJumping();
}

void APlayerCharacter::Move(const FInputActionValue &Value)
{
    if (isAlive && canMove) {
        float moveActionValue = Value.Get<float>();
        FVector direction = FVector(1.0f, 0.0f, 0.0f);
        AddMovementInput(direction, moveActionValue);
        
        UpdateDirection(moveActionValue);
    }
}

void APlayerCharacter::UpdateDirection(float moveDirection)
{
    FRotator currentRotation = Controller->GetControlRotation();
    if (moveDirection < 0.0f) {
        if (currentRotation.Yaw != 180.0f) {
            Controller->SetControlRotation(FRotator(currentRotation.Pitch, 180.0f, currentRotation.Roll));
        }
    } else if (moveDirection > 0.0f) {
        if (currentRotation.Yaw != 0.0f) {
            Controller->SetControlRotation(FRotator(currentRotation.Pitch, 0.0f, currentRotation.Roll));
        }
    }
}

void APlayerCharacter::OnAttackOverrideAnimEnd(bool completed)
{
    canAttack = true;
    canMove = true;
    
    // The AttackCollisionBox is now disabled via AnimNotifyStates
}

void APlayerCharacter::AttackBoxOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult)
{
    AEnemy* enemy = Cast<AEnemy>(OtherActor);
    if (enemy) {
        // take damage
        enemy->ApplyDamage(AttackDamage, AttackStunDuration);
    }
}

void APlayerCharacter::EnableAttackCollisionBox(bool enabled)
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

void APlayerCharacter::ApplyDamage(int DamageAmount, float StunDuration)
{
    if (!isAlive) return;
    
    UpdateHitPoints(hitPoints - DamageAmount);
    
    UPaperZDAnimInstance *anim = GetAnimInstance();
    if (hitPoints <= 0)
    {
        // is dead
        canMove = false;
        isAlive = false;
        canAttack = false;
        
        UpdateHitPoints(0);
        
        anim->JumpToNode(FName("JumpDie"), FName("CaptainStateMachine"));
        EnableAttackCollisionBox(false);
    } else {
        // is alive still
        anim->JumpToNode(FName("JumpTakeHit"), FName("CaptainStateMachine"));
    }
}

void APlayerCharacter::UpdateHitPoints(int NewHitPoints)
{
    hitPoints = NewHitPoints;
}
