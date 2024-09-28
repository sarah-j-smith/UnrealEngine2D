// (c) 2024 Sarah Smith

#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawning();
}

// Called every frame
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
	check(enemy);
}
