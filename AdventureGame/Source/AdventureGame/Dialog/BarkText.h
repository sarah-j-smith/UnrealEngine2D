// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TimerHandle.h"
#include "BarkText.generated.h"

class UBarkLine;
class UVerticalBox;
class USphereComponent;

#define G_MAX_BARK_LINES 3
#define BARK_LINE_DELAY 3.0f

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UBarkText : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBarkLine> BarkLineClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox *BarkContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor BarkTextColor = FColor::Orange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* BarkPosition;

	UFUNCTION(BlueprintCallable)
	void SetText(FText NewText);

	UFUNCTION(BlueprintCallable)
	void ClearText();

	UFUNCTION(BlueprintCallable)
	void SetBarkLines(const TArray<FText> &NewBarkLines);

	UFUNCTION()
	void AddQueuedBarkLine();

	FTimerHandle BarkLineTimer;
	
private:
	TArray<FText> BarkLines;

	void SetBarkLineTimer();

	void ClearBarkLineTimer();

	void DumpBarkText();
};
