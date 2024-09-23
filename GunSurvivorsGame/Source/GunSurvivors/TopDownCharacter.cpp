// (c) 2024 Sarah Smith


#include "TopDownCharacter.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComp);

	characterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	characterFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
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
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, moveValue.ToString() + TEXT(" - MoveTriggered"));
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue &Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("MoveCompleted"));
}

void ATopDownCharacter::Shoot(const FInputActionValue &Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Shoot"));
}