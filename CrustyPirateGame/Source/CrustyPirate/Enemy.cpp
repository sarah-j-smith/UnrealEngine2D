// (c) 2024 Sarah Smith


#include "Enemy.h"

AEnemy::AEnemy() {
    PrimaryActorTick.bCanEverTick = true;
    
    playerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
    playerDetectorSphere->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    
    playerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
    playerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
}

void AEnemy::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    
    if (isAlive && followTarget) {
        float moveDirection = (followTarget->GetActorLocation().X - GetActorLocation().X) > 0.0 ? 1.0f : -1.0f;
        if (canMove) {
            FVector worldDirection = FVector(1.0f, 0.0f, 0.0f);
            AddMovementInput(worldDirection, moveDirection);
        }
    }
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent *OverlappedComponent,
                                  AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                                  int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {
    APlayerCharacter *playerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (playerCharacter) {
        followTarget = playerCharacter;
    }
}

void AEnemy::DetectorOverlapEnd(
                                UPrimitiveComponent *OverlappedComponent,
                                AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                                int32 OtherBodyIndex) {
    APlayerCharacter *playerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (playerCharacter) {
        followTarget = NULL;
    }
}
