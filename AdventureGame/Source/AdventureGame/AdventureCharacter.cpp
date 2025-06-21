// (c) 2025 Sarah Smith

#include "AdventureCharacter.h"

#include "AdventureGame.h"
#include "AdvGameUtils.h"
#include "BarkText.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

AAdventureCharacter::AAdventureCharacter()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventureCharacter - Player movement controlled by AI"));

	BarkTextComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BarkTextComponent"));
	BarkTextComponent->SetupAttachment(RootComponent);
}

void AAdventureCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BarkTextComponent->SetWidgetClass(BarkTextClass);
	BarkTextComponent->SetWidgetSpace(EWidgetSpace::Screen);
	BarkTextComponent->SetDrawSize(BarkTextSize);
}

void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	TargetPlayerLocation = CapsuleComp->GetComponentLocation();

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureCharacter::BeginPlay"));

	SetupCamera();

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
		UE_LOG(LogAdventureGame, Verbose, TEXT("Camera loaded - level size: %s"), *(Camera->CameraConfines->GetLocalBounds().ToString()));
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("AAdventureCharacter::SetupCamera failed - misplaced or no camera set in this level?"));
	}
}
