// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "AdventureGameHUD.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

bool HasChangedMuch(const FVector2D& Current, const FVector2D& Previous)
{
	return (fabs(Previous.X - Current.X) >= DBL_EPSILON || fabs(Previous.Y - Current.Y) >= DBL_EPSILON);
}

AAdventureCharacter::AAdventureCharacter()
{
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	TargetPlayerLocation = CapsuleComp->GetComponentLocation();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	FVector2D Velocity = FVector2D(MovementComponent->Velocity.X, MovementComponent->Velocity.Y);
	if (HasChangedMuch(Velocity, LastVelocity))
	{
		LastVelocity = Velocity;
		if (Velocity.SquaredLength() > 0)
		{
			LastNonZeroMovement = Velocity;
		}
	}
}

void AAdventureCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputComponent)
	{
		InputComponent->BindAction(PointAndClickInput, ETriggerEvent::Triggered, this,
		                           &AAdventureCharacter::HandlePointAndClick);
	}
}

void AAdventureCharacter::HandlePointAndClick(const FInputActionValue& Value)
{
	bool isPressed;
	float locationX, locationY;
	FVector MouseWorldLocation, MouseWorldDirection;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->GetInputTouchState(ETouchIndex::Type::Touch1, locationX, locationY, isPressed);
	if (isPressed)
	{
		PlayerController->DeprojectScreenPositionToWorld(locationX, locationY, MouseWorldLocation, MouseWorldDirection);
	}
	else
	{
		isPressed = PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	}

	// Do not set the Target Player Location if the mouse is over the UI - not the play area.
	if (!GamePlayArea.Contains(FIntPoint(MouseWorldLocation.X, MouseWorldLocation.Y)))
	{
		UE_LOG(LogInput, Warning, TEXT("HandlePointAndClick ignoring mouse click: %f %f %f"), MouseWorldLocation.X, MouseWorldLocation.Y,
		   MouseWorldLocation.Z);
		return;
	};
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
	TargetPlayerLocation = FVector(floorf(MouseWorldLocation.X), floorf(MouseWorldLocation.Y), CapsuleLocation.Z);

	UE_LOG(LogInput, Warning, TEXT("Mouse click: %f %f %f"), MouseWorldLocation.X, MouseWorldLocation.Y,
	       MouseWorldLocation.Z);
}
