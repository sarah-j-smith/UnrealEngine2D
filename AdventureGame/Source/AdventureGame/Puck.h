// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Puck.generated.h"

DECLARE_MULTICAST_DELEGATE(FPointAndClickDelegate);

UCLASS()
class ADVENTUREGAME_API APuck : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APuck();

	// MARK: Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputAction* PointAndClickInput = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputMappingContext* InputMappingContext = nullptr;

	UFUNCTION(BlueprintCallable, Category = Input)
	void HandlePointAndClick(const FInputActionValue &Value);

	FPointAndClickDelegate PointAndClickDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when Pawn is possessed
	virtual void PawnClientRestart() override;
};
