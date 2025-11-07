#pragma once

// Required Includes
#include "Engine/DataAsset.h"

// Generated Includes
#include "EpicJam_Player_InputConfig.generated.h"

// Forward Declarations
class UInputAction;

UCLASS()
class EPICJAM_API UEpicJam_Player_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
	
    //============================
    // Variables
    //============================
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Look;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Interact;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Crouch;
};