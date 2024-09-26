// (c) 2024 Sarah Smith


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(sphereComp);

	bulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));
	bulletSprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isLaunched && movementSpeed > 0.0) {
		FVector currentLocation = GetActorLocation();
		FVector2D movementVector = moveDirection * movementSpeed * DeltaTime;
		FVector newLocation = currentLocation + FVector(movementVector.X, 0.0, movementVector.Y);
		SetActorLocation(newLocation);
	}
}

void ABullet::Launch(FVector2D direction, float speed)
{
	if (isLaunched) return;

	isLaunched = true;

	moveDirection = direction;
	movementSpeed = speed;

	float DeleteTime = 10.0;

	GetWorldTimerManager().SetTimer(
		deleteTimer, this, 
		&ABullet::OnDeleteTimerTimeout, 1.0, false, DeleteTime);
}

void ABullet::OnDeleteTimerTimeout()
{
	Destroy();
}