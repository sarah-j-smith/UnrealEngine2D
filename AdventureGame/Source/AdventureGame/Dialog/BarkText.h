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
#define BARK_LINE_WIDTH 30

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UBarkText : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBarkLine> BarkLineClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox *BarkContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BarkLineDisplayTime = BARK_LINE_DELAY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCharactersPerLine = BARK_LINE_WIDTH;

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
	bool IsBarking = false;

	bool bWarningShown = false;

	UPROPERTY()
	AActor *ViewTarget = nullptr;
	
	TArray<FText> BarkLines;

	void SetBarkLineTimer();

	void ClearBarkLineTimer();

	void DumpBarkText();
};
