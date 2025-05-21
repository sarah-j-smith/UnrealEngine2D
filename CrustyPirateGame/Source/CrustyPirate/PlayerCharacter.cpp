// (c) 2024 Sarah Smith


#include "PlayerCharacter.h"

#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
    
    UGameInstance *gameInstance = GetGameInstance();
    MyGameInstance = Cast<UCrustyPirateGameInstance>(gameInstance);
    if (MyGameInstance) {
        hitPoints = MyGameInstance->PlayerHP;
        
        if (MyGameInstance->IsDoubleJumpUnlocked)
        {
            UnlockDoubleJump();
        }
    }

    SetupPlayerHUD();
}

void APlayerCharacter::SetupPlayerHUD()
{
    if (PlayerHUDClass) {
        APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        PlayerHUDWidget = CreateWidget<UPlayerHUD>(playerController, PlayerHUDClass);
        if (PlayerHUDWidget)
        {
            PlayerHUDWidget->AddToPlayerScreen();
            PlayerHUDWidget->SetHP(hitPoints);
            PlayerHUDWidget->SetDiamonds(MyGameInstance->DiamondCount);
            PlayerHUDWidget->SetLevel(MyGameInstance->CurrentLevelIndex);
        }
    }

#if PLATFORM_IOS
    IosSetup();
#endif
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
        inputComponent->BindAction(quitAction, ETriggerEvent::Started, this,
                                   &APlayerCharacter::GameQuitPressed);
    }
}


void APlayerCharacter::Stun(float DurationInSeconds) {
    isStunned = true;
    GetWorldTimerManager().SetTimer(
        stunTimer, this,
        &APlayerCharacter::OnStunTimerTimeout, 1.0, false, DurationInSeconds);
    
    GetAnimInstance()->StopAllAnimationOverrides();
    EnableAttackCollisionBox(false);
}

void APlayerCharacter::OnStunTimerTimeout() {
    isStunned = false;
}


void APlayerCharacter::OnRestartGameTimerTimeout()
{
    MyGameInstance->RestartGame();
}


void APlayerCharacter::GameQuitPressed(const FInputActionValue &Value)
{
    APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (MyGameInstance && playerController) {
        MyGameInstance->QuitGame(playerController);
    }
}

void APlayerCharacter::Attack(const FInputActionValue &Value)
{
    AttackTrigger();
}

void APlayerCharacter::AttackTrigger()
{
    UE_LOG(LogTemp, Warning, TEXT("AttackTrigger function"));
    if (isAlive && canAttack && !isStunned) {
        canAttack = false;
        canMove = false;
        
        UPaperZDAnimInstance *anim = GetAnimInstance();
        anim->PlayAnimationOverride(attackAnimSequence, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
    }
}

void APlayerCharacter::JumpStarted(const FInputActionValue &Value)
{
    JumpStartTrigger();
}

void APlayerCharacter::JumpEnded(const FInputActionValue &Value)
{
    JumpEndTrigger();
}

void APlayerCharacter::JumpStartTrigger()
{
    if (isAlive && canMove && !isStunned) {
        Jump();
    }
}

void APlayerCharacter::JumpEndTrigger()
{
    StopJumping();
}

void APlayerCharacter::Move(const FInputActionValue &Value)
{
    if (isAlive && canMove && !isStunned) {
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
    if (!isActive || !isAlive) return;
    
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
    if (!isAlive || !isActive) return;
    
    Stun(StunDuration);
    
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
        
        GetWorldTimerManager().SetTimer(
            RestartGameTimer, this,
            &APlayerCharacter::OnRestartGameTimerTimeout, 1.0, false, GameOverDelay);
    } else {
        // is alive still
        anim->JumpToNode(FName("JumpTakeHit"), FName("CaptainStateMachine"));
    }
}

void APlayerCharacter::UpdateHitPoints(int NewHitPoints)
{
    hitPoints = NewHitPoints;
    PlayerHUDWidget->SetHP(hitPoints);
    MyGameInstance->UpdateHitPoints(hitPoints);
}

void APlayerCharacter::CollectItem(CollectableType ItemType)
{
    UGameplayStatics::PlaySound2D(GetWorld(), ItemPickupSound);
    
    switch (ItemType) {
        case CollectableType::Diamond:
            MyGameInstance->AddDiamond(1);
            PlayerHUDWidget->SetDiamonds(MyGameInstance->DiamondCount);
            break;
            
        case CollectableType::HealthPotion:
            // Heal player
            // UpdateHitPoints(max(hitPoints + HealthPotionHealing, maxHitPoints));
            UpdateHitPoints(hitPoints + HealthPotionHealing);
            break;
            
        case CollectableType::DoubleJumpUpgrade:
            if (!MyGameInstance->IsDoubleJumpUnlocked)
            {
                MyGameInstance->IsDoubleJumpUnlocked = true;
                UnlockDoubleJump();
            }
            break;
            
        default:
            break;
    }
}

void APlayerCharacter::UnlockDoubleJump()
{
    JumpMaxCount = 2;
}

void APlayerCharacter::Deactivate()
{
    if (isActive)
    {
        isActive = false;
        canMove = false;
        canAttack = false;
        
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void APlayerCharacter::IosSetup()
{
    if (ControlsHUDClass) {
        APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        ControlsHUDWidget = CreateWidget<UControlsHUD>(playerController, ControlsHUDClass);
        if (ControlsHUDWidget)
        {
            ControlsHUDWidget->AddToViewport(100);
            UE_LOG(LogTemp, Display, TEXT("binding ios buttons"));
            ControlsHUDWidget->AttackButton->OnClicked.AddDynamic(this, &APlayerCharacter::AttackTrigger);
            ControlsHUDWidget->JumpButton->OnPressed.AddDynamic(this, &APlayerCharacter::JumpStartTrigger);
            ControlsHUDWidget->JumpButton->OnReleased.AddDynamic(this, &APlayerCharacter::JumpEndTrigger);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Could not create ControlsHUDWidget!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not bind iOS HUD buttons. Did you forget to set ControlsHUDClass in PlayerCharacter blueprint?"));
    }
}
