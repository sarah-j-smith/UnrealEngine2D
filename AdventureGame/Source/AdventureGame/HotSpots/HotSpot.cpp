// (c) 2025 Sarah Smith


#include "HotSpot.h"

#include "../Constants.h"
#include "../AdventureGame.h"
#include "../Player/AdventureCharacter.h"
#include "../Player/AdventurePlayerController.h"

#include "Kismet/GameplayStatics.h"

AHotSpot::AHotSpot()
	: AStaticMeshActor()
{
	// This is not a "real" sphere - its not a mesh, its just a collision volume defined by dimensions
	WalkToPoint = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	WalkToPoint->SetupAttachment(RootComponent);
	WalkToPoint->SetSphereRadius(4.0f);

}

void AHotSpot::BeginPlay()
{
	HotSpotType = IsPickup() ? "PickUp" : "HotSpot";
	HotSpotName = ShortDescription.IsEmpty() ? GetClass()->GetName() : ShortDescription.ToString();
	UStaticMeshComponent* StaticMeshComponent = GetStaticMeshComponent();
	if (StaticMeshComponent && StaticMeshComponent->GetStaticMesh())
	{
		SetEnableMeshComponent(true);
		Super::OnBeginCursorOver.AddDynamic(this, &AHotSpot::OnBeginCursorOver);
		Super::OnEndCursorOver.AddDynamic(this, &AHotSpot::OnEndCursorOver);
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("%s %s - BeginPlay()- static mesh NOT valid"), *HotSpotType, *HotSpotName);
	}
	Super::BeginPlay();

	const APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	WalkToPosition = WalkToPoint->GetComponentLocation();
	WalkToPosition.Z = PlayerPawn->GetActorLocation().Z;

	UItemDataAsset *UseAsset = OnUseSuccessItem.LoadSynchronous();
	if (UItemDataAsset *GiveAsset = OnUseSuccessItem.LoadSynchronous(); UseAsset || GiveAsset)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PlayerController);
		UseAsset->SetAdventurePlayerController(Apc);
		GiveAsset->SetAdventurePlayerController(Apc);
	}
}

void AHotSpot::OnBeginCursorOver(AActor *TouchedActor)
{
	// GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::White, TEXT("HotSpot::OnBeginCursorOver()"),
	// 							 false, FVector2D(2.0, 2.0));

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("OnBeginCursorOver"));
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PC); IsValid(APC))
	{
		APC->MouseEnterHotSpot(this);
	}
}

void AHotSpot::OnEndCursorOver(AActor *TouchedActor)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("OnEndCursorOver"));
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PC); IsValid(APC))
	{
		APC->MouseLeaveHotSpot();
	}
}

void AHotSpot::OnClose_Implementation()
{
	IVerbInteractions::OnClose_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On close"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "CloseDefaultText"));
}

void AHotSpot::OnOpen_Implementation()
{
	IVerbInteractions::OnOpen_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On open"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "OpenDefaultText"));
}

void AHotSpot::OnGive_Implementation()
{
	IVerbInteractions::OnGive_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On give"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "GiveDefaultText"));
}

void AHotSpot::OnPickUp_Implementation()
{
	IVerbInteractions::OnPickUp_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Pickup"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "PickUpDefaultText"));
}

void AHotSpot::OnTalkTo_Implementation()
{
	IVerbInteractions::OnTalkTo_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On talk to"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "TalkToDefaultText"));
}

void AHotSpot::OnLookAt_Implementation()
{
	IVerbInteractions::OnLookAt_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On look at"));
	BarkAndEnd(Description.IsEmpty() ? LOCTABLE(ITEM_STRINGS_KEY, "LookAtDefaultText") : Description);
}

void AHotSpot::OnPull_Implementation()
{
	IVerbInteractions::OnPull_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On pull"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "PullDefaultText"));
}

void AHotSpot::OnPush_Implementation()
{
	IVerbInteractions::OnPush_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On push"));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "PushDefaultText"));
}

void AHotSpot::OnUse_Implementation()
{
	// HotSpot Use is mostly relevant for doors.  eg "Use Door"
	// and that is covered in the ADoor::OnUse_Implementation override.
	// If we have an object in the scene we want to use somehow, like a computer
	// terminal or a water-fountain then a custom script would need to be done.
	IVerbInteractions::OnUse_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On use from AHotSpot default implement."));
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "UseDefaultText"));
}

void AHotSpot::OnWalkTo_Implementation()
{
	IVerbInteractions::OnWalkTo_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On walk to"));
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PC); IsValid(APC))
	{
		if (APC->IsAlreadyAtHotspotClicked())
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("Subject"), ShortDescription);
			APC->PlayerBark(FText::Format(LOCTABLE(ITEM_STRINGS_KEY, "HotSpotWalkAlreadyAt"), Args));
		}
		else
		{
			APC->PlayerBark(LOCTABLE(ITEM_STRINGS_KEY, "HotSpotWalkArrived"));;
		}
		APC->InterruptCurrentAction();
	}
}

void AHotSpot::OnItemUsed_Implementation()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Item Used"));
	if (UItemDataAsset *ItemDataAsset = OnUseSuccessItem.LoadSynchronous())
	{
		APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (const AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PC))
		{
			// Item was used on this hotspot, and the kind of that item matches the
			// recipe in the ItemDataAsset. 
			if (Apc->SourceItem->ItemKind == ItemDataAsset->SourceItem)
			{
				ItemDataAsset->OnItemUseSuccess();
				return;
			}
		}
	}
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemUsedDefaultText"));
}

void AHotSpot::OnItemGiven_Implementation()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Item Given"));
	if (UItemDataAsset *ItemDataAsset = OnGiveSuccessItem.LoadSynchronous())
	{
		APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (const AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PC))
		{
			if (Apc->SourceItem->ItemKind == ItemDataAsset->SourceItem)
			{
				ItemDataAsset->OnItemGiveSuccess();
				return;
			}
		}
	}
	BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemGivenDefaultText"));
}

AActor *AHotSpot::SpawnAtPlayerLocation(TSubclassOf<AActor> SpawnClass, float Scale, float Lifetime)
{
	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	FVector PlayerLocation = PlayerPawn->GetTransform().GetLocation();
	FVector SpawnLocation(PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z + ZOffsetForSpawn);
	check(SpawnClass)
	AActor* Item = GetWorld()->SpawnActor<AActor>(
		SpawnClass,
		SpawnLocation,
		SpawnOrientation);
	Item->SetLifeSpan(Lifetime);
	Item->SetActorScale3D(FVector(Scale, Scale, Scale));

	return Item;
}

void AHotSpot::Hide()
{
	if (HotSpotHidden) return;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	HotSpotHidden = true;
}

void AHotSpot::Show()
{
	if (!HotSpotHidden) return;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	HotSpotHidden = false;
}

void AHotSpot::SetEnableMeshComponent(bool Enabled)
{
	if (Enabled)
	{
		UStaticMeshComponent* StaticMeshComponent = GetStaticMeshComponent();
		if (StaticMeshComponent && StaticMeshComponent->GetStaticMesh())
		{
			UE_LOG(LogAdventureGame, Verbose, TEXT("%s %s static mesh is valid & enabled."), *HotSpotType, *HotSpotName);
			// StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
			StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
			StaticMeshComponent->SetGenerateOverlapEvents(true);
		}
	}
	else
	{
		UStaticMeshComponent* StaticMeshComponent = GetStaticMeshComponent();
		if (StaticMeshComponent && StaticMeshComponent->GetStaticMesh())
		{
			StaticMeshComponent->SetCollisionProfileName("NoCollision");
			StaticMeshComponent->SetVisibility(false);
			UE_LOG(LogAdventureGame, Verbose, TEXT("%s %s - static mesh is valid but disabled"), *HotSpotType, *HotSpotName);
		}
	}
}

void AHotSpot::Bark(const FText &Text) const
{
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PC); IsValid(APC))
	{
		APC->PlayerBark(Text);
	}
}

void AHotSpot::BarkAndEnd(const FText& Text) const
{
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PC); IsValid(APC))
	{
		APC->PlayerBark(Text);
		APC->InterruptCurrentAction();
	}
}
