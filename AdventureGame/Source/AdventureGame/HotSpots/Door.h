// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "HotSpot.h"
#include "PickUp.h"
#include "AdventureGame/Enums/DoorState.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API ADoor : public APickUp
{
	GENERATED_BODY()
public:
	virtual EVerbType CheckForDefaultCommand() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName LevelToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName DoorLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	EDoorState DoorState;

	UFUNCTION(BlueprintCallable, Category = Door)
	bool UnlockDoor();

	UFUNCTION(BlueprintCallable, Category = Door)
	bool LockDoor();

	UFUNCTION(BlueprintCallable, Category = Door)
	bool OpenDoor();

	UFUNCTION(BlueprintCallable, Category = Door)
	bool CloseDoor();

	virtual void OnUse_Implementation() override;

	virtual void OnOpen_Implementation() override;
};
