// (c) 2025 Sarah Smith


#include "FollowCamera.h"

#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

AFollowCamera::AFollowCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCameraBase = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	SetRootComponent(FollowCameraBase);
	    
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
    
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	CameraConfines = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraConfines"));
	CameraConfines->SetupAttachment(FollowCameraBase);
}

// Called when the game starts or when spawned
void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();

	CameraConfines->SetBoxExtent(ConfinesOfCamera);
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

