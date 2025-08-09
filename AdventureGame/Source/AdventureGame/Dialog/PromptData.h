// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "PromptData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ADVENTUREGAME_API FPromptData: public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    bool CanBeShown() const;

    /**
     * Mark this prompt as not visible. It will not be shown to the player. Various
     * actions or events could hide a prompt, for example if a character took an
     * action that closed off a branch of the story.
     */
    void Hide();

    bool IsIndex(int PromptIndex, int SubIndex) const;

    int32 NextSubIndex() const
    {
        return PromptSubNumber + 1;
    }

    /// Top level index for the prompt
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Indexes")
    int32 PromptNumber = 0;
    
    /// Subordinate level index for the prompt. 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Indexes")
    int32 PromptSubNumber = 0;

    /// Is the prompt visible to the player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visibility")
    bool Visible = true;

    /// Is the prompt shown as selected to the player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visibility")
    bool HasBeenSelected = false;

    /// Does the prompt disappear after one use. Use this with the
    /// PromptSubNumber to have new prompt replace this after its used.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visibility")
    bool SingleUse = false;

    /// List of texts the player will bark
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PromptText")
    TArray<FText> PromptText;

    /// List of texts the NPC will say in response
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PromptText")
    TArray<FText> NPCResponse;

    /// Table of topics to switch to
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UDataTable* SwitchTopic = nullptr;

    /// Does selecting this prompt cause the conversation to finish
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool EndsConversation = false;

    /// Some event that gets fired on selection
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName Event;
};
