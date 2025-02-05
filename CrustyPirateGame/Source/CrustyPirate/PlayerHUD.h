// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock *HPText;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock *DiamondsText;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock *LevelText;
    
    void SetHP(int hitPoints);
    
    void SetDiamonds(int amount);
    
    void SetLevel(int level);
};
