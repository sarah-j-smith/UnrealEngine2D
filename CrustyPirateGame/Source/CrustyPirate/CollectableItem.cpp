// (c) 2024 Sarah Smith


#include "CollectableItem.h"

#include "PlayerCharacter.h"

ACollectableItem::ACollectableItem()
{
	PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    SetRootComponent(CapsuleComponent);
    
    ItemFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ItemFlipbook"));
    ItemFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();
	
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectableItem::OverlapBegin);
}

// Called every frame
void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableItem::OverlapBegin(UPrimitiveComponent *OverlappedComponent,
                           AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                           int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult)
{
    APlayerCharacter *playerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (playerCharacter && playerCharacter->isAlive) {
        playerCharacter->CollectItem(ItemType);
        Destroy();
    }
}
