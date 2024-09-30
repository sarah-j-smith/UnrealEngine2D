// (c) 2024 Sarah Smith


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComp);

	enemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	enemyFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanFollow && playerCharacter) {
		FVector currentLocation = GetActorLocation();
		FVector playerLocation = playerCharacter->GetActorLocation();
		FVector chaseVector = playerLocation - currentLocation;
		float distanceToPlayer = chaseVector.Length();
		if (distanceToPlayer > StopDistance) {
			chaseVector.Normalize();
			FVector movementVector = chaseVector * DeltaTime * MovementSpeed;
			FVector newLocation = currentLocation + movementVector;
			SetActorLocation(newLocation);
		}

		currentLocation = GetActorLocation();
		playerLocation = playerCharacter->GetActorLocation();
		FVector FlipbookScale = enemyFlipbook->GetComponentScale();
		// Enemy is to the right of the player so needs to look left - scale.x === -1
		if (playerLocation.X < currentLocation.X) {
			if (FlipbookScale.X > 0) {
				enemyFlipbook->SetWorldScale3D(FVector(-1.0, 1.0, 1.0));
			}
		} else if (playerLocation.X > currentLocation.X ) {
			if (FlipbookScale.X < 0) {
				enemyFlipbook->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
			}
		}
	}
}

void AEnemy::Die() 
{
	if (!IsAlive) return;

	IsAlive = false;
	CanFollow = false;

	enemyFlipbook->SetFlipbook(deadFlipbookAsset);
	enemyFlipbook->SetTranslucentSortPriority(-5);

	float DestroyTime = 10.0f;
	GetWorldTimerManager().SetTimer(
		destroyTimer,
		this, &AEnemy::OnDestroyTimerTimeout,
		1.0f, false, DestroyTime
	);
	enemyDiedDelegate.Broadcast();
}

void AEnemy::OnDestroyTimerTimeout() {
	Destroy();
}