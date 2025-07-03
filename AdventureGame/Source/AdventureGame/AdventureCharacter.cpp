// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "AdventureGame.h"
#include "AdvGameUtils.h"
#include "BarkText.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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
	OnSitOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnSitAnimOverrideEnd);
	OnClimbOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnClimbAnimOverrideEnd);
	OnInteractOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnInteractAnimOverrideEnd);
	OnTurnLeftOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnTurnLeftAnimOverrideEnd);
	OnTurnRightOverrideEndDelegate.BindUObject(this, &AAdventureCharacter::OnTurnRightAnimOverrideEnd);
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
		if (bBarkTextConfinesNeedsUpdate)
		{
			UpdateBarkTextConfines();
		}
		ConstrainBarkText();
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
	check(PrevLocation.Z >= MinZValue);

	// in prod work around via forcing to MinZValue
	float ZValue = std::max(PrevLocation.Z, 5.0);
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

void AAdventureCharacter::PlayerBark(const FText &NewBarkText)
{
	if (bBarkTimerActive)
	{
		ClearBark();
	}
	BarkText->SetText(NewBarkText);
	BarkTextComponent->SetVisibility(true);
	bBarkTextConfinesNeedsUpdate = true;
	GetWorldTimerManager().SetTimer(BarkTimerHandle, this, &AAdventureCharacter::BarkTimerTimeout, 1.0, false, BarkDelay);
	bBarkTimerActive = true;
}

void AAdventureCharacter::ClearBark()
{
	BarkText->SetText(FText::GetEmpty());
	BarkTextComponent->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(BarkTimerHandle);
	bBarkTimerActive = false;
}

void AAdventureCharacter::BarkTimerTimeout()
{
	ClearBark();
	bBarkTextConfinesNeedsUpdate = true;
	UpdateBarkTextConfines();
}

void AAdventureCharacter::ConstrainBarkText()
{
	FVector IdealBarkLocation = GetCapsuleComponent()->GetComponentLocation() + BarkRelativeOffset;
	FVector NewBarkPosition;
	NewBarkPosition.X = UKismetMathLibrary::FClamp(IdealBarkLocation.X, UpdatedBarkConfineMin.X, UpdatedBarkConfineMax.X);
	NewBarkPosition.Y = UKismetMathLibrary::FClamp(IdealBarkLocation.Y, UpdatedBarkConfineMin.Y, UpdatedBarkConfineMax.Y);
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
		UE_LOG(LogAdventureGame, Verbose, TEXT("### Camera loaded - level size: %s"), *(Camera->CameraConfines->GetLocalBounds().ToString()));
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
	UE_LOG(LogAdventureGame, Log, TEXT("### AAdventureCharacter::InitializeBarkTextConfines - GetSceneBounds - ConfineMax: %s - ConfineMin: %s"),
		*(BarkConfineMax.ToString()), *(BarkConfineMin.ToString()));

	UE_LOG(LogAdventureGame, Log, TEXT("### BarkTextSize: %s"), *(BarkTextSize.ToString()));
}

void AAdventureCharacter::UpdateBarkTextConfines()
{
	if (!bBarkTextConfinesNeedsUpdate) return;
	if (!BarkTextComponent) return;
	bBarkTextConfinesNeedsUpdate = false;

	auto Geometry = BarkText->GetCachedGeometry();
	auto LocalSize = Geometry.GetLocalSize();
	auto ScreenPosition = Geometry.LocalToAbsolute(FVector2D(0, 0)); //TopLeft
	auto ScreenSize = Geometry.LocalToAbsolute(LocalSize) - ScreenPosition;
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UpdateBarkTextConfines - Screen Size w x h: %f x %f"), ScreenSize.X, ScreenSize.Y);

	auto WorldContextObject = GetWorld();
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("### UpdateBarkTextConfines - ViewportSize w x h: %f x %f"), ViewportSize.X, ViewportSize.Y);

	float WidgetScale = ViewportSize.X / (fabs(BarkConfineMax.X) + fabs(BarkConfineMin.X));
	
	float halfWidth = fabs(ScreenSize.X) / (WidgetScale * 2);
	float height = fabs(ScreenSize.Y) / WidgetScale;
	
	/// The anchor point for the bark text is at the bottom center of the rectangle of
	/// the text. The text grows upward if it is multiple lines.
	UpdatedBarkConfineMax = BarkConfineMax - FVector(halfWidth, height, 0.0);
	UpdatedBarkConfineMin = BarkConfineMin + FVector(halfWidth, 0, 0.0);
	
	UE_LOG(LogAdventureGame, Log, TEXT("### UpdateBarkTextConfines - UpdatedBarkConfineMax: %s - UpdatedBarkConfineMin: %s"),
		*(UpdatedBarkConfineMax.ToString()), *(UpdatedBarkConfineMin.ToString()));
}
