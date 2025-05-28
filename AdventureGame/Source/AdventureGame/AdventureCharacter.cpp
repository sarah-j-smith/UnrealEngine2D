// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

AAdventureCharacter::AAdventureCharacter()
	: TargetPlayerLocation(FVector::ZeroVector)
	, PointAndClickInput(nullptr)
	, InputMappingContext(nullptr)
{
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	TargetPlayerLocation = CapsuleComp->GetComponentLocation();

	APlayerController *PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem *SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem) {
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// For now do nothing
	// Controller->GetPathFollowingAgent();
}

void AAdventureCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
	UEnhancedInputComponent *InputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputComponent) {
		InputComponent->BindAction(PointAndClickInput, ETriggerEvent::Triggered, this, &AAdventureCharacter::HandlePointAndClick);
	}
}

void AAdventureCharacter::HandlePointAndClick(const FInputActionValue& Value)
{
	bool isPressed;
	float locationX, locationY;
	FVector MouseWorldLocation, MouseWorldDirection;

	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->GetInputTouchState(ETouchIndex::Type::Touch1, locationX, locationY, isPressed);
	if (isPressed) {
		PlayerController->DeprojectScreenPositionToWorld(locationX, locationY, MouseWorldLocation, MouseWorldDirection);
	} else
	{
		isPressed = PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
	TargetPlayerLocation = FVector(floorf(MouseWorldLocation.X), floorf(MouseWorldLocation.Y), CapsuleLocation.Z);

	UE_LOG(LogInput, Warning, TEXT("Mouse click: %f %f %f"), MouseWorldLocation.X, MouseWorldLocation.Y, MouseWorldLocation.Z);
}
