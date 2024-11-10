// (c) 2024 Sarah Smith

#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"


AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase *gameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	if (gameModeBase) {
		mainGameMode = Cast<AMainGameMode>(gameModeBase);
	}

	AActor *playerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
	if (playerActor)
	{
		playerCharacter = Cast<ATopDownCharacter>(playerActor);
		playerCharacter->playerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
	}
	StartSpawning();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::OnSpawnTimerTimeOut()
{
	SpawnEnemy();
}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(
		spawnTimer, this,
		&AEnemySpawner::OnSpawnTimerTimeOut, spawnDelay, true, spawnDelay);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(spawnTimer);
}

void AEnemySpawner::SpawnEnemy()
{
	// This Vrand function is not all that efficient really.  Maybe could do better with
	// basic trigonometry.
	FVector2D randomPosition = FVector2D(FMath::VRand());
	randomPosition.Normalize();
	randomPosition *= spawnDistance;
	FVector spawnLocation = FVector(randomPosition.X, 0.0f, randomPosition.Y) + GetActorLocation();

	AEnemy *enemy = GetWorld()->SpawnActor<AEnemy>(
		EnemyActorToSpawn,
		spawnLocation,
		FRotator::ZeroRotator);
	SetupEnemy(enemy);

	// Difficulty system
	totalEnemyCount += 1;
	if (totalEnemyCount % difficultySpikeInterval == 0) {
		// Hit interval
		if (spawnDelay > spawnTimeMinimumLimit) {
			spawnDelay -= decreaseSpawnTimerByEveryInterval;
			if (spawnDelay < spawnTimeMinimumLimit) {
				spawnDelay = spawnTimeMinimumLimit;
			}
		}
	}
}

void AEnemySpawner::SetupEnemy(AEnemy *enemy)
{
	if (enemy) {
		enemy->playerCharacter = playerCharacter;
		enemy->CanFollow = true;
		enemy->enemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
}

void AEnemySpawner::OnEnemyDied()
{
	const int AMOUNT_TO_ADD = 10;
	mainGameMode->AddScore(AMOUNT_TO_ADD);
}

void AEnemySpawner::OnPlayerDied()
{
	StopSpawning();

	TArray<AActor *> enemyArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemyArray);
	for (AActor *actor : enemyArray) {
		AEnemy *enemy = Cast<AEnemy>(actor);
		if (enemy) {
			enemy->CanFollow = false;
		}
	}

	mainGameMode->RestartGame();
}