#pragma once

// Required Includes
#include "GameFramework/Pawn.h"

// Engine Includes
// Plugin Includes
#include "InputActionValue.h"
#include "MoverSimulationTypes.h"
#include "MoverExamplesCharacter.h"

// Module Includes
// Game Includes

// Generated Header
#include "EpicJam_PlayerPawn.generated.h"

//============================
// Enums
//============================

//============================
// Structs
//============================

// Forward declarations

// Static variables

struct FInputActionValue;

UCLASS(Blueprintable)
//class EPICJAM_API AEpicJam_PlayerPawn : public AMoverExamplesCharacter
class EPICJAM_API AEpicJam_PlayerPawn : public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()
	
	//============================
	// Variables
	//============================
	
public:

protected:
	/** Holds functionality for nav movement data and functions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category="Nav Movement")
	TObjectPtr<UNavMoverComponent> NavMoverComponent;
	
private:
	FVector LastAffirmativeMoveInput = FVector::ZeroVector;	// Movement input (intent or velocity) the last time we had one that wasn't zero

	FVector CachedMoveInputIntent = FVector::ZeroVector;
	FVector CachedMoveInputVelocity = FVector::ZeroVector;

	FRotator CachedTurnInput = FRotator::ZeroRotator;
	FRotator CachedLookInput = FRotator::ZeroRotator;

	bool bIsJumpJustPressed = false;
	bool bIsJumpPressed = false;
	bool bIsFlyingActive = false;
	bool bShouldToggleFlying = false;

	bool bIsCrouched = false;

	
	//============================
	// Constructor
	//============================
	
public:
	AEpicJam_PlayerPawn(const FObjectInitializer& ObjectInitializer);


	//============================
	// API
	//============================
public:


	//============================
	// Inherited
	//============================

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	
	//============================
	// IMoverInputProducerInterface
	//============================

protected:
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

	//============================
	// Implementation
	//============================
	
protected:
	// Override this function in native class to author input for the next simulation frame. Consider also calling Super method.
	virtual void OnProduceInput(float DeltaMs, FMoverInputCmdContext& OutInputCmd);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Input", DisplayName = "OnMove")
	void BP_Move(const FVector& movement);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Input", DisplayName = "OnLook")
	void BP_Look(const FVector2D& Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Input", DisplayName = "OnJump")
	void BP_Jump(bool bJumpPressed);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Input", DisplayName = "OnInteract")
	void BP_Interact();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Input", DisplayName = "OnCrouch")
	void BP_Crouch(bool bCrouched);

private:


	//============================
	// Event Handlers
	//============================
	
protected:
	void OnMoveTriggered(const FInputActionValue& value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLookTriggered(const FInputActionValue& value);
	void OnLookCompleted(const FInputActionValue& value);
	void OnJumpStarted(const FInputActionValue& value);
	void OnJumpReleased(const FInputActionValue& value);
	void OnInteract(const FInputActionValue& value);
	void OnCrouchTriggered(const FInputActionValue& value);
	void OnCrouchCompleted(const FInputActionValue& value);

	//============================
	// Debug
	//============================
private:

};