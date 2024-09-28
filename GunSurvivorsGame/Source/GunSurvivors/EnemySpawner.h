// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/TimerHandle.h"
#include "Enemy.h"

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

	FTimerHandle spawnTimer;

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnSpawnTimerTimeOut();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();
};
