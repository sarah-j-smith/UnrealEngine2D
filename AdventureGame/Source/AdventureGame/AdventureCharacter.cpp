// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "AdventureGame.h"
#include "AdvGameUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

AAdventureCharacter::AAdventureCharacter()
{
	UE_LOG(LogAdventureGame, Log, TEXT("*** Construct: AAdventureCharacter - Player movement controlled by AI"));
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	TargetPlayerLocation = CapsuleComp->GetComponentLocation();

	UE_LOG(LogAdventureGame, Log, TEXT("*** Construct: AAdventureCharacter - Player movement controlled by AI"));

	SetupCamera();
}

void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This is used just to feed in the PaperZD Set direction for the sprite facing
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	FVector2D Velocity = FVector2D(MovementComponent->Velocity.X, MovementComponent->Velocity.Y);
	if (AdvGameUtils::HasChangedMuch(Velocity, LastVelocity))
	{
		LastVelocity = Velocity;
		if (Velocity.SquaredLength() > 0)
		{
			LastNonZeroMovement = Velocity;
		}
	}
}

void AAdventureCharacter::SetPosition(const FVector& NewPosition)
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
	TargetPlayerLocation = FVector(floorf(NewPosition.X), floorf(NewPosition.Y), CapsuleLocation.Z);
}

void AAdventureCharacter::SetFacingDirection(EWalkDirection Direction)
{
	switch (Direction)
	{
	case EWalkDirection::Left:
		LastNonZeroMovement = FVector2D(-1.0f, 0.0f);
		break;
	case EWalkDirection::Right:
		LastNonZeroMovement = FVector2D(1.0f, 0.0f);
		break;
	case EWalkDirection::Up:
		LastNonZeroMovement = FVector2D(0.0f, -1.0f);
		break;
	case EWalkDirection::Down:
		LastNonZeroMovement = FVector2D(0.0f, 1.0f);
	default:
		UE_LOG(LogAdventureGame, Warning, TEXT("Unsupported case in SetFacingDirection"));
		break;
	}
}

void AAdventureCharacter::SetupCamera()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);

	FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
	FVector SpawnLocation(CapsuleLocation.X, 0.0, 0.0);

	check(CameraActorToSpawn);
	AFollowCamera* Camera = GetWorld()->SpawnActor<AFollowCamera>(
		CameraActorToSpawn,
		SpawnLocation,
		FRotator::ZeroRotator);

	Camera->PlayerCharacter = this;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTarget(Camera);
}

