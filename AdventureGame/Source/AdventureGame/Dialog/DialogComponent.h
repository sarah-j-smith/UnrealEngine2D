// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "ConversationData.h"
#include "AdventureGame/Constants.h"
#include "Components/ActorComponent.h"
#include "DialogComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationDataLoad, UDialogComponent *, DialogComponent);
DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationDataSave, UDialogComponent *, DialogComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVENTUREGAME_API UDialogComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDialogComponent();
    
    // Copy the data out of the TopicList tables into local ConversationData arrays
    UFUNCTION(BlueprintCallable, Category = "HotSpot")
    void FillConversationData();

    // Mark this prompt has been selected by the player.
    UFUNCTION(BlueprintCallable, Category = "HotSpot")
    void UpdatePromptAtIndex(int32 TopicIndex, int32 PromptIndex);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    FConversationDataLoad ConversationDataLoad;
    FConversationDataSave ConversationDataSave;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialog")
    FLinearColor TextColor = G_NPC_Default_Text_Colour;

    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    TArray<FConversationData> ConversationData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    int32 TopicIndex = 0;
    
    /// Data tables that contain the dialogue at design time, in the editor.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    TArray<UDataTable *> TopicList;

    void LoadPrompts(TArray<FPromptData> &PromptsToShow);

    int ConversationCount() const;
private:
    

};
