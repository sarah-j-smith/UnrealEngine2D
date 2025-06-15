// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "AdventureGame.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"

bool HasChangedMuch(const FVector2D& Current, const FVector2D& Previous)
{
	return (fabs(Previous.X - Current.X) >= DBL_EPSILON || fabs(Previous.Y - Current.Y) >= DBL_EPSILON);
}

AAdventureCharacter::AAdventureCharacter()
{
	CreateCameraDelegate.BindUObject(this, &AAdventureCharacter::SetupCamera);
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	TargetPlayerLocation = CapsuleComp->GetComponentLocation();
	
	GetWorldTimerManager().SetTimerForNextTick(CreateCameraDelegate);
}

void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This is used just to feed in the PaperZD Set direction for the sprite facing
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
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

	UE_LOG(LogAdventureGame, Warning, TEXT("*** SetupPlayerInputComponent"));

	if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		InputComponent->BindAction(PointAndClickInput, ETriggerEvent::Completed, this,
		                           &AAdventureCharacter::HandlePointAndClick);
		InputComponent->BindAction(PointAndClickInput, ETriggerEvent::Triggered, this,
								   &AAdventureCharacter::HandlePointAndClick);
		InputComponent->BindAction(PointAndClickInput, ETriggerEvent::Started, this,
							   &AAdventureCharacter::HandlePointAndClick);

		UE_LOG(LogAdventureGame, Warning, TEXT("*** BINDINGS DONE"));
	}
}

void AAdventureCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	
	UE_LOG(LogAdventureGame, Warning, TEXT("*** PawnClientRestart"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer());
		if (SubSystem)
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			SubSystem->ClearAllMappings();
			// Add each mapping context, along with their priority values. Higher values take priority over lower values.
			SubSystem->AddMappingContext(InputMappingContext, 0);
			UE_LOG(LogAdventureGame, Warning, TEXT("*** Success: [re-]bound the input mapping context"));
		}
	}
}

void AAdventureCharacter::HandlePointAndClick(const FInputActionValue& Value)
{
	GetAdventurePlayerController()->HandlePointAndClickInput();
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
	UE_LOG(LogAdventureGame, Warning, TEXT("SetupCamera"));
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (!CapsuleComp)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("FAIL: Cannot SetupFollowCamera - UCapsuleComponent is not defined"));
		return;
	}

	FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
	FVector SpawnLocation(CapsuleLocation.X, 0.0, 0.0);

	if (!IsValid(CameraActorToSpawn) || CameraActorToSpawn.GetDefaultObject() == nullptr)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("FAIL: Cannot SetupFollowCamera - CameraActorToSpawn is not defined"));
		return;
	}

	AFollowCamera* Camera = GetWorld()->SpawnActor<AFollowCamera>(
		CameraActorToSpawn,
		SpawnLocation,
		FRotator::ZeroRotator);

	Camera->PlayerCharacter = this;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTarget(Camera);
}

AAdventurePlayerController* AAdventureCharacter::GetAdventurePlayerController() const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	if (!AdventurePlayerController)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("Failed to get AAdventurePlayerController"));
	}
	return AdventurePlayerController;
}
