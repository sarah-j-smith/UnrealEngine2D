// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComp);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);

	carSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CarSprite"));
	carSprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent *inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (inputComponent) {
		inputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}
}

void APlayerCharacter::Move(const FInputActionValue &Value)
{
	FVector2D moveActionValue = Value.Get<FVector2D>();
	// GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, moveActionValue.ToString(), false, FVector2D(2.0, 2.0));
	if (canMove) {
		if (abs(moveActionValue.Y) > 0.0) {
			const float deltaTime = GetWorld()->DeltaTimeSeconds;
			const FVector currentLocation = GetActorLocation();
			const FVector directionVector = GetActorUpVector();
			const FVector moveVector = directionVector * moveSpeed * deltaTime * moveActionValue.Y;
			const FVector newLocation = currentLocation + moveVector;
			SetActorLocation(newLocation);
		}
	}
}