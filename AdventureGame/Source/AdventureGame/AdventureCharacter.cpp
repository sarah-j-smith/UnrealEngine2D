// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "AdventureGame.h"
#include "AdvGameUtils.h"
#include "BarkText.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

AAdventureCharacter::AAdventureCharacter()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventureCharacter - Player movement controlled by AI"));

	BarkTextComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BarkTextComponent"));
	BarkTextComponent->SetupAttachment(RootComponent);

	OnClimbOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnInteractAnimOverrideEnd);
	OnInteractOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnInteractAnimOverrideEnd);
}

void AAdventureCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BarkTextComponent->SetWidgetClass(BarkTextClass);
	BarkTextComponent->SetWidgetSpace(EWidgetSpace::Screen);
	BarkTextComponent->SetDrawSize(BarkTextSize);
	BarkRelativeOffset = BarkTextComponent->GetComponentLocation() - GetCapsuleComponent()->GetComponentLocation();
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureCharacter::BeginPlay"));

	check(BarkTextComponent);
	UUserWidget* ComponentWidget = BarkTextComponent->GetWidget();
	check(ComponentWidget);
	BarkText = Cast<UBarkText>(ComponentWidget);
	check(BarkText);
	BarkText->SetText(FText::GetEmpty());
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

	if (bBarkTimerActive)
	{
		ConstrainBarkText();
	}
}

void AAdventureCharacter::Climb()
{
	// Play the attack animation override - this triggers an anim notify state that enables the
	// attack collision box when the animation is in the correct frame. This box colliding with
	// the player is what causes actual damage to be done to the player
	UPaperZDAnimInstance *anim = GetAnimInstance();
	anim->PlayAnimationOverride(ClimbAnimationSequence,
		FName("DefaultSlot"), 1.0f, 0.0f, OnClimbOverrideEndDelegate);
}

void AAdventureCharacter::Interact()
{
	// Play the attack animation override - this triggers an anim notify state that enables the
	// attack collision box when the animation is in the correct frame. This box colliding with
	// the player is what causes actual damage to be done to the player
	UPaperZDAnimInstance *anim = GetAnimInstance();
	anim->PlayAnimationOverride(InteractAnimationSequence,
		FName("DefaultSlot"), 1.0f, 0.0f,
		OnInteractOverrideEndDelegate);
}

void AAdventureCharacter::OnInteractAnimOverrideEnd(bool completed)
{
	//
}

void AAdventureCharacter::OnClimbAnimOverrideEnd(bool completed)
{
	//
}

void AAdventureCharacter::TeleportToLocation(FVector NewLocation)
{
	UCapsuleComponent *Capsule = GetCapsuleComponent();
	FVector PrevLocation = Capsule->GetComponentLocation();
	Capsule->SetWorldLocation(FVector(NewLocation.X, NewLocation.Y, PrevLocation.Z));
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

void AAdventureCharacter::PlayerBark(FText NewBarkText)
{
	if (bBarkTimerActive)
	{
		ClearBark();
	}
	BarkText->SetText(NewBarkText);
	GetWorldTimerManager().SetTimer(BarkTimerHandle, this, &AAdventureCharacter::BarkTimerTimeout, 1.0, false, BarkDelay);
	bBarkTimerActive = true;
}

void AAdventureCharacter::ClearBark()
{
	BarkText->SetText(FText::GetEmpty());
	GetWorldTimerManager().ClearTimer(BarkTimerHandle);
	bBarkTimerActive = false;
}

void AAdventureCharacter::BarkTimerTimeout()
{
	ClearBark();
}

void AAdventureCharacter::ConstrainBarkText()
{
	FVector IdealBarkLocation = GetCapsuleComponent()->GetComponentLocation() + BarkRelativeOffset;
	FVector NewBarkPosition;
	NewBarkPosition.X = UKismetMathLibrary::FClamp(IdealBarkLocation.X, BarkConfineMin.X, BarkConfineMax.X);
	NewBarkPosition.Y = UKismetMathLibrary::FClamp(IdealBarkLocation.Y, BarkConfineMin.Y, BarkConfineMax.Y);
	NewBarkPosition.Z = IdealBarkLocation.Z;
	BarkTextComponent->SetWorldLocation(NewBarkPosition);
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
	InitializeBarkTextConfines(Camera);
}

void AAdventureCharacter::InitializeBarkTextConfines(AFollowCamera *Camera)
{
	Camera->GetSceneBounds(BarkConfineMax, BarkConfineMin);

	/// The anchor point for the bark text is at the bottom center of the rectangle of
	/// the text. The text grows upward if it is multiple lines.
	BarkConfineMax -= FVector(BarkTextSize.X / 2.0, BarkTextSize.Y, 0.0);
	BarkConfineMin += FVector(BarkTextSize.X / 2.0, 0, 0.0);
}