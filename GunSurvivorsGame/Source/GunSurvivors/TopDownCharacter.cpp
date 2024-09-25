// (c) 2024 Sarah Smith


#include "TopDownCharacter.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComp);

	characterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	characterFlipbook->SetupAttachment(RootComponent);

	gunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	gunParent->SetupAttachment(RootComponent);

	gunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	gunSprite->SetupAttachment(gunParent);

	bulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	bulletSpawnPosition->SetupAttachment(gunSprite);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController *playerController = Cast<APlayerController>(Controller);
	if (playerController) {
		playerController->SetShowMouseCursor(true);
		
		UEnhancedInputLocalPlayerSubsystem *subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
		if (subSystem) {
			subSystem->AddMappingContext(inputMappingContext, 0);
		}
	}
}

bool ATopDownCharacter::IsInMapBoundsHorizontal(float xpos)
{
	bool Result = true;
	Result = (mapLimitsHorizontal.X < xpos && xpos < mapLimitsHorizontal.Y);
	return Result;
}

bool ATopDownCharacter::IsInMapBoundsVertical(float zpos)
{
	bool Result = true;
	Result = (mapLimitsVertical.X < zpos && zpos < mapLimitsVertical.Y);
	return Result;
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canMove) {
		if (movementDirection.SquaredLength() > 0.0) {
			if (movementDirection.SquaredLength() > 1.0) {
				movementDirection.Normalize();
			}
			FVector2D moveVector = movementDirection * movementSpeed * DeltaTime;
			FVector currentLocation = GetActorLocation();
			FVector newLocation = currentLocation + FVector(moveVector.X, 0.0, 0.0);
			if (!IsInMapBoundsHorizontal(newLocation.X)) {
				newLocation -= FVector(moveVector.X, 0.0, 0.0);
			}
			newLocation += FVector(0.0, 0.0, moveVector.Y);
			if (!IsInMapBoundsVertical(newLocation.Z)) {
				newLocation -= FVector(0.0, 0.0, moveVector.Y);
			}
			SetActorLocation(newLocation);
		}
	}

	APlayerController *playerController = Cast<APlayerController>(Controller);
	if (playerController) {
		FVector MouseWorldLocation, MouseWorldDirection;
		playerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector currentLocation = GetActorLocation();
		FVector start = FVector(currentLocation.X, 0.0, currentLocation.Z);
		FVector target = FVector(MouseWorldLocation.X, 0.0, MouseWorldLocation.Z);
		FRotator gunParentRotator = UKismetMathLibrary::FindLookAtRotation(start, target);

		gunParent->SetRelativeRotation(gunParentRotator);
	}
}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent *enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::MoveTriggered);
	enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::MoveCompleted);
	enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Canceled, this, &ATopDownCharacter::MoveCompleted);

	enhancedInputComponent->BindAction(shootAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Shoot);
	enhancedInputComponent->BindAction(shootAction, ETriggerEvent::Started, this, &ATopDownCharacter::Shoot);
}


void ATopDownCharacter::MoveTriggered(const FInputActionValue &Value)
{
	FVector2D moveValue = Value.Get<FVector2D>();

	if (canMove) {
		movementDirection = moveValue;
		characterFlipbook->SetFlipbook(runFlipbook);

		FVector flipbookScale = characterFlipbook->GetComponentScale();
		if (movementDirection.X < 0.0) {
			if (flipbookScale.X > 0.0) {
				characterFlipbook->SetWorldScale3D(FVector(-1.0, 1.0, 1.0));
			}
		} else if (movementDirection.X > 0.0) {
			if (flipbookScale.X < 0.0) {
				characterFlipbook->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
			}
		}
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue &Value)
{
	movementDirection = FVector2D::ZeroVector;
	characterFlipbook->SetFlipbook(idleFlipbook);
}

void ATopDownCharacter::Shoot(const FInputActionValue &Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Shoot"));
}