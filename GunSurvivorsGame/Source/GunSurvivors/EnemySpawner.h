// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/TimerHandle.h"

#include "Enemy.h"
#include "TopDownCharacter.h"
#include "MainGameMode.h"

#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnDistance = 400.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int totalEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int difficultySpikeInterval = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnTimeMinimumLimit = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float decreaseSpawnTimerByEveryInterval = 0.05;

	FTimerHandle spawnTimer;

	ATopDownCharacter *playerCharacter;

	AMainGameMode *mainGameMode;

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnSpawnTimerTimeOut();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();
	void SetupEnemy(AEnemy *enemy);

	UFUNCTION()
	void OnEnemyDied();

	UFUNCTION()
	void OnPlayerDied();
};
