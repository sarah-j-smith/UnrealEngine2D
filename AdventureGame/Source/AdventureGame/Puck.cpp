// (c) 2025 Sarah Smith


#include "Puck.h"

#include "AdventureGame.h"
#include "EnhancedInputComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APuck::APuck()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogAdventureGame, Log, TEXT("*** Construct: APuck - receives Player Input clicks"));
}

void APuck::HandlePointAndClick(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, TEXT("APuck clicked"),
								 false, FVector2D(2.0, 2.0));

	UE_LOG(LogAdventureGame, Log, TEXT("*** APuck::HandlePointAndClick Broadcast"));
	PointAndClickDelegate.Broadcast();
}

// Called when the game starts or when spawned
void APuck::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAdventureGame, Log, TEXT("*** APuck::BeginPlay"));
}

void APuck::PawnClientRestart()
{
	Super::PawnClientRestart();

	UE_LOG(LogAdventureGame, Log, TEXT("*** APuck::PawnClientRestart"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer());
		if (SubSystem)
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			SubSystem->ClearAllMappings();
			// Add each mapping context, along with their priority values. Higher values take priority over lower values.
			SubSystem->AddMappingContext(InputMappingContext, 0);
			FString PCClass = PlayerController->GetClass()->GetName();
			UE_LOG(LogAdventureGame, Log, TEXT("*** Success: APuck::PawnClientRestart [re-]bound the input mapping context to: %s"), *PCClass);
		}
	}
}

// Called to bind functionality to input
void APuck::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogAdventureGame, Verbose, TEXT("*** APuck::SetupPlayerInputComponent"));

	if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		InputComponent->BindAction(PointAndClickInput, ETriggerEvent::Triggered, this,
								   &APuck::HandlePointAndClick);
		UE_LOG(LogAdventureGame, Log, TEXT("*** APuck::SetupPlayerInputComponent INPUT BINDINGS DONE"));
	}
}

