#pragma once

// Required Includes
#include "GameFramework/PlayerController.h"

// Engine Includes
// Plugin Includes
// Module Includes
// Game Includes

// Generated Header
#include "EpicJam_PlayerController.generated.h"

//============================
// Enums
//============================

//============================
// Structs
//============================

// Forward declarations
struct FInputActionValue;
class UInputMappingContext;
class UEpicJam_Player_InputConfig;

// Static variables

UCLASS(Blueprintable)
class EPICJAM_API AEpicJam_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	//============================
	// Variables
	//============================
public:
	/// Config for available input actions - create custom InputConfig object to insert here.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UEpicJam_Player_InputConfig* InputActions;

	///Expose a mapping context as a property in your header file...
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MoverExamples)
	float MouseSensitivity = 1.0f;

	// Whether or not we author our movement inputs relative to whatever base we're standing on, or leave them in world space. Only applies if standing on a base of some sort.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MoverExamples)
	bool bUseBaseRelativeMovement = true;
	
	/// If true, rotate the Character toward the direction the actor is moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MoverExamples)
	bool bOrientRotationToMovement = true;
	
	/// If true, the actor will continue orienting towards the last intended orientation (from input) even after movement intent input has ceased.
	/// This makes the character finish orienting after a quick stick flick from the player.  If false, character will not turn without input. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoverExamples)
	bool bMaintainLastInputOrientation = false;

	
	//============================
	// Constructor
	//============================
public:
	AEpicJam_PlayerController(const FObjectInitializer& ObjectInitializer);


	//============================
	// API
	//============================
public:


	//============================
	// Inherited
	//============================

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;


	//============================
	// Implementation
	//============================

private:

	//============================
	// Event Handlers
	//============================
protected:


	//============================
	// Debug
	//============================
private:

};