// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "../AdventureGame.h"
#include "../HUD/AdvGameUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

AAdventureCharacter::AAdventureCharacter()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventureCharacter - Player movement controlled by AI"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(4.0);
	Sphere->SetRelativeLocation(FVector(0, -20, 0));
	
	OnClimbOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnInteractAnimOverrideEnd);
	OnInteractOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnInteractAnimOverrideEnd);
}

void AAdventureCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OnSitOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnSitAnimOverrideEnd);
	OnClimbOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnClimbAnimOverrideEnd);
	OnInteractOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnInteractAnimOverrideEnd);
	OnTurnLeftOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnTurnLeftAnimOverrideEnd);
	OnTurnRightOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnTurnRightAnimOverrideEnd);

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AdventureCharacter on construct"));
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureCharacter::BeginPlay"));
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

void AAdventureCharacter::Climb(EInteractTimeDirection InteractTimeDirection)
{
	LastInteractTimeDirection = InteractTimeDirection;
	UPaperZDAnimInstance *Anim = GetAnimInstance();
	Anim->PlayAnimationOverride(LastNonZeroMovement.X > 0 ? ClimbRightAnimationSequence : ClimbLeftAnimationSequence,
		FName("DefaultSlot"),
		InteractTimeDirection == EInteractTimeDirection::Forward ? 1.0f : -1.0f,
		0.0f,
		OnClimbOverrideEndDelegate);
}

void AAdventureCharacter::Interact(EInteractTimeDirection InteractTimeDirection)
{
	LastInteractTimeDirection = InteractTimeDirection;
	UPaperZDAnimInstance *Anim = GetAnimInstance();
	Anim->PlayAnimationOverride(LastNonZeroMovement.X > 0 ? InteractRightAnimationSequence : InteractLeftAnimationSequence,
		FName("DefaultSlot"),
		InteractTimeDirection == EInteractTimeDirection::Forward ? 1.0f : -1.0f,
		0.0f,
		OnInteractOverrideEndDelegate);
}

void AAdventureCharacter::Sit(EInteractTimeDirection InteractTimeDirection)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureCharacter::Sit - %s"),
		*(TimeDirectionGetDescriptiveString(InteractTimeDirection)));

	LastInteractTimeDirection = InteractTimeDirection;
	UPaperZDAnimInstance *Anim = GetAnimInstance();
	Anim->PlayAnimationOverride(
		LastNonZeroMovement.X > 0 ? SitRightAnimationSequence : SitLeftAnimationSequence,
		FName("DefaultSlot"),
		InteractTimeDirection == EInteractTimeDirection::Forward ? 1.0f : -1.0f,
		0.0f,
		OnSitOverrideEndDelegate);
}

void AAdventureCharacter::TurnLeft(EInteractTimeDirection InteractTimeDirection)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureCharacter::TurnLeft - %s"),
		*(TimeDirectionGetDescriptiveString(InteractTimeDirection)));

	LastInteractTimeDirection = InteractTimeDirection;
	UPaperZDAnimInstance *Anim = GetAnimInstance();
	Anim->PlayAnimationOverride(
		TurnLeftAnimationSequence,
		FName("DefaultSlot"),
		InteractTimeDirection == EInteractTimeDirection::Forward ? 1.0f : -1.0f,
		0.0f,
		OnTurnLeftOverrideEndDelegate);
}

void AAdventureCharacter::TurnRight(EInteractTimeDirection InteractTimeDirection)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureCharacter::TurnRight - %s"),
		*(TimeDirectionGetDescriptiveString(InteractTimeDirection)));

	LastInteractTimeDirection = InteractTimeDirection;
	UPaperZDAnimInstance *Anim = GetAnimInstance();
	Anim->PlayAnimationOverride(
		TurnRightAnimationSequence,
		FName("DefaultSlot"),
		InteractTimeDirection == EInteractTimeDirection::Forward ? 1.0f : -1.0f,
		0.0f,
		OnTurnRightOverrideEndDelegate);
}

void AAdventureCharacter::OnInteractAnimOverrideEnd(bool completed)
{
	AnimationCompleteDelegate.Broadcast(EInteractionType::Interact, completed);
}

void AAdventureCharacter::OnClimbAnimOverrideEnd(bool completed)
{
	AnimationCompleteDelegate.Broadcast(EInteractionType::Climb, completed);
}

void AAdventureCharacter::OnSitAnimOverrideEnd(bool completed)
{
	AnimationCompleteDelegate.Broadcast(EInteractionType::Sit, completed);
}

void AAdventureCharacter::OnTurnLeftAnimOverrideEnd(bool completed)
{
	if (completed)
	{
		if (LastInteractTimeDirection == EInteractTimeDirection::Forward)
		{
			SetFacingDirection(EWalkDirection::Left);
		}
		else
		{
			SetFacingDirection(EWalkDirection::Down);
		}
	}
	AnimationCompleteDelegate.Broadcast(EInteractionType::TurnLeft, completed);
}

void AAdventureCharacter::OnTurnRightAnimOverrideEnd(bool completed)
{
	if (completed)
	{
		if (LastInteractTimeDirection == EInteractTimeDirection::Forward)
		{
			SetFacingDirection(EWalkDirection::Right);
		}
		else
		{
			SetFacingDirection(EWalkDirection::Down);
		}
	}
	AnimationCompleteDelegate.Broadcast(EInteractionType::TurnRight, completed);
}

void AAdventureCharacter::TeleportToLocation(FVector NewLocation)
{
	UCapsuleComponent *Capsule = GetCapsuleComponent();
	FVector PrevLocation = Capsule->GetComponentLocation();

	// in debug stop here to find out why player is falling through the floor
	// probably a misplaced door or player start object.
	if (PrevLocation.Z >= MinZValue)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("AAdventureCharacter::TeleportToLocation - %s"), *NewLocation.ToString());
	}

	// in prod work around via forcing to MinZValue
	float ZValue = std::max(static_cast<float>(NewLocation.Z), MinZValue);
	SetActorLocation(FVector(NewLocation.X, NewLocation.Y, PrevLocation.Z));
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
	}
}

EWalkDirection AAdventureCharacter::GetFacingDirection()
{
	if (LastNonZeroMovement.X > 0.0f)
	{
		return EWalkDirection::Right;
	}
	else if (LastNonZeroMovement.X < 0.0f)
	{
		return EWalkDirection::Left;
	}
	else if (LastNonZeroMovement.Y > 0.0f)
	{
		return EWalkDirection::Up;
	}
	return EWalkDirection::Down;
}

void AAdventureCharacter::PlayerBark(FText TextToBark)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PlayerController))
	{
		// Apc->AdventureHUDWidget->
	}
}

void AAdventureCharacter::ClearBark()
{
}

void AAdventureCharacter::SetupCamera()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);

	AActor *CameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), AFollowCamera::StaticClass());
	AFollowCamera *Camera = Cast<AFollowCamera>(CameraActor);
	if (IsValid(Camera))
	{
		Camera->PlayerCharacter = this;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetViewTarget(Camera);
		Camera->SetupCamera();
		UE_LOG(LogAdventureGame, Verbose, TEXT("Camera loaded - level size: %s"), *(Camera->CameraConfines->GetLocalBounds().ToString()));
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("AAdventureCharacter::SetupCamera failed - misplaced or no camera set in this level?"));
	}
}
