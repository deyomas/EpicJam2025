// Header Include
#include "EpicJam_PlayerPawn.h"

// Engine Includes
#include "EnhancedInputComponent.h"
#include "EpicJam_PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"

// Game Includes
#include "EpicJam.h"
#include "EpicJam_Player_InputConfig.h"

//============================
// Constructor
//============================

AEpicJam_PlayerPawn::AEpicJam_PlayerPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

//============================
// Inherited
//============================

void AEpicJam_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Do whatever you want here. By now we have the latest movement state and latest input processed.

	// Spin camera based on input
	if (AEpicJam_PlayerController* PC = Cast<AEpicJam_PlayerController>(GetController()))
	{
		// Simple input scaling. A real game will probably map this to an acceleration curve
		static float LookRateYaw = 100.f * PC->MouseSensitivity;	// degs/sec
		static float LookRatePitch = 100.f * PC->MouseSensitivity;	// degs/sec

		PC->AddYawInput(CachedLookInput.Yaw * LookRateYaw * DeltaTime);
		PC->AddPitchInput(-CachedLookInput.Pitch * LookRatePitch * DeltaTime);
	}

	// Clear all camera-related cached input
	CachedLookInput = FRotator::ZeroRotator;
}

void AEpicJam_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (USpringArmComponent* SpringComp = FindComponentByClass<USpringArmComponent>())
	{
		// This is not best practice: do not search for component every frame
		SpringComp->bUsePawnControlRotation = true;
	}
}

void AEpicJam_PlayerPawn::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	//UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	//Input->BindAction(AimingInputAction, ETriggerEvent::Triggered, this, &AFooBar::SomeCallbackFunc);
	UE_LOG(EpicJam, Log, TEXT("Pawn is setting up player input component"));
	
	AEpicJam_PlayerController* pc = Cast<AEpicJam_PlayerController>(GetController());
	if (!IsValid(pc)) return;
	
	UEpicJam_Player_InputConfig* inputActions = pc->InputActions;
	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(playerInputComponent);
	UE_LOG(EpicJam, Log, TEXT("Pawn has valid controller"));
	if(!IsValid(inputActions)) return; // Don't want to bind to invalid UInputActions!
	UE_LOG(EpicJam, Log, TEXT("Pawn knows controller's InputActions are valid"));
	inputComp->BindAction(inputActions->Move, ETriggerEvent::Triggered, this, &AEpicJam_PlayerPawn::OnMoveTriggered);
	inputComp->BindAction(inputActions->Move, ETriggerEvent::Completed, this, &AEpicJam_PlayerPawn::OnMoveCompleted);
	inputComp->BindAction(inputActions->Look, ETriggerEvent::Triggered, this, &AEpicJam_PlayerPawn::OnLookTriggered);
	inputComp->BindAction(inputActions->Look, ETriggerEvent::Completed, this, &AEpicJam_PlayerPawn::OnLookCompleted);
	inputComp->BindAction(inputActions->Jump, ETriggerEvent::Started, this, &AEpicJam_PlayerPawn::OnJumpStarted);
	inputComp->BindAction(inputActions->Jump, ETriggerEvent::Completed, this, &AEpicJam_PlayerPawn::OnJumpReleased);
	inputComp->BindAction(inputActions->Interact, ETriggerEvent::Started, this, &AEpicJam_PlayerPawn::OnInteract);
	inputComp->BindAction(inputActions->Crouch, ETriggerEvent::Started, this, &AEpicJam_PlayerPawn::OnCrouchTriggered);
	inputComp->BindAction(inputActions->Crouch, ETriggerEvent::Completed, this, &AEpicJam_PlayerPawn::OnCrouchCompleted);
}

//============================
// IMoverInputProducerInterface
//============================

void AEpicJam_PlayerPawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	//IMoverInputProducerInterface::ProduceInput_Implementation(SimTimeMs, InputCmdResult);

	OnProduceInput((float)SimTimeMs, InputCmdResult);
}

//============================
// Implementation
//============================

void AEpicJam_PlayerPawn::OnProduceInput(float DeltaMs, FMoverInputCmdContext& OutInputCmd)
{
	// Generate user commands. Called right before the Character movement simulation will tick (for a locally controlled pawn)
	// This isn't meant to be the best way of doing a camera system. It is just meant to show a couple of ways it may be done
	// and to make sure we can keep distinct the movement, rotation, and view angles.
	// Styles 1-3 are really meant to be used with a gamepad.
	//
	// Its worth calling out: the code that happens here is happening *outside* of the Character movement simulation. All we are doing
	// is generating the input being fed into that simulation. That said, this means that A) the code below does not run on the server
	// (and non controlling clients) and B) the code is not rerun during reconcile/resimulates. Use this information guide any
	// decisions about where something should go (such as aim assist, lock on targeting systems, etc): it is hard to give absolute
	// answers and will depend on the game and its specific needs. In general, at this time, I'd recommend aim assist and lock on 
	// targeting systems to happen /outside/ of the system, i.e, here. But I can think of scenarios where that may not be ideal too.

	FCharacterDefaultInputs& CharacterInputs = OutInputCmd.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

	if (GetController() == nullptr)
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
		{
			static const FCharacterDefaultInputs DoNothingInput;
			// If we get here, that means this pawn is not currently possessed and we're choosing to provide default do-nothing input
			CharacterInputs = DoNothingInput;
		}

		// We don't have a local controller so we can't run the code below. This is ok. Simulated proxies will just use previous input when extrapolating
		return;
	}

	if (USpringArmComponent* SpringComp = FindComponentByClass<USpringArmComponent>())
	{
		// This is not best practice: do not search for component every frame
		SpringComp->bUsePawnControlRotation = true;
	}

	CharacterInputs.ControlRotation = FRotator::ZeroRotator;

	AEpicJam_PlayerController* PC = Cast<AEpicJam_PlayerController>(GetController());
	if (!IsValid(PC))
		return;
	
	if (PC)
	{
		CharacterInputs.ControlRotation = PC->GetControlRotation();
	}

	bool bRequestedNavMovement = false;
	if (NavMoverComponent)
	{
		bRequestedNavMovement = NavMoverComponent->ConsumeNavMovementData(CachedMoveInputIntent, CachedMoveInputVelocity);
	}
	
	// Favor velocity input 
	bool bUsingInputIntentForMove = CachedMoveInputVelocity.IsZero();

	if (bUsingInputIntentForMove)
	{
		const FVector FinalDirectionalIntent = CharacterInputs.ControlRotation.RotateVector(CachedMoveInputIntent);
		CharacterInputs.SetMoveInput(EMoveInputType::DirectionalIntent, FinalDirectionalIntent);
	}
	else
	{
		CharacterInputs.SetMoveInput(EMoveInputType::Velocity, CachedMoveInputVelocity);
	}

	// Normally cached input is cleared by OnMoveCompleted input event but that won't be called if movement came from nav movement
	if (bRequestedNavMovement)
	{
		CachedMoveInputIntent = FVector::ZeroVector;
		CachedMoveInputVelocity = FVector::ZeroVector;
	}
	
	static float RotationMagMin(1e-3);

	const bool bHasAffirmativeMoveInput = (CharacterInputs.GetMoveInput().Size() >= RotationMagMin);
	
	// Figure out intended orientation
	CharacterInputs.OrientationIntent = FVector::ZeroVector;


	if (bHasAffirmativeMoveInput)
	{
		if (PC->bOrientRotationToMovement)
		{
			// set the intent to the actors movement direction
			CharacterInputs.OrientationIntent = CharacterInputs.GetMoveInput().GetSafeNormal();
		}
		else
		{
			// set intent to the the control rotation - often a player's camera rotation
			CharacterInputs.OrientationIntent = CharacterInputs.ControlRotation.Vector().GetSafeNormal();
		}

		LastAffirmativeMoveInput = CharacterInputs.GetMoveInput();
	}
	else if (PC->bMaintainLastInputOrientation)
	{
		// There is no movement intent, so use the last-known affirmative move input
		CharacterInputs.OrientationIntent = LastAffirmativeMoveInput;
	}
	
	CharacterInputs.bIsJumpPressed = bIsJumpPressed;
	CharacterInputs.bIsJumpJustPressed = bIsJumpJustPressed;

	if (bShouldToggleFlying)
	{
		if (!bIsFlyingActive)
		{
			CharacterInputs.SuggestedMovementMode = DefaultModeNames::Flying;
		}
		else
		{
			CharacterInputs.SuggestedMovementMode = DefaultModeNames::Falling;
		}

		bIsFlyingActive = !bIsFlyingActive;
	}
	else
	{
		CharacterInputs.SuggestedMovementMode = NAME_None;
	}

	// Convert inputs to be relative to the current movement base (depending on options and state)
	CharacterInputs.bUsingMovementBase = false;

	if (PC->bUseBaseRelativeMovement)
	{
		if (const UCharacterMoverComponent* MoverComp = GetComponentByClass<UCharacterMoverComponent>())
		{
			if (UPrimitiveComponent* MovementBase = MoverComp->GetMovementBase())
			{
				FName MovementBaseBoneName = MoverComp->GetMovementBaseBoneName();

				FVector RelativeMoveInput, RelativeOrientDir;

				UBasedMovementUtils::TransformWorldDirectionToBased(MovementBase, MovementBaseBoneName, CharacterInputs.GetMoveInput(), RelativeMoveInput);
				UBasedMovementUtils::TransformWorldDirectionToBased(MovementBase, MovementBaseBoneName, CharacterInputs.OrientationIntent, RelativeOrientDir);

				CharacterInputs.SetMoveInput(CharacterInputs.GetMoveInputType(), RelativeMoveInput);
				CharacterInputs.OrientationIntent = RelativeOrientDir;

				CharacterInputs.bUsingMovementBase = true;
				CharacterInputs.MovementBase = MovementBase;
				CharacterInputs.MovementBaseBoneName = MovementBaseBoneName;
			}
		}
	}

	// Clear/consume temporal movement inputs. We are not consuming others in the event that the game world is ticking at a lower rate than the Mover simulation. 
	// In that case, we want most input to carry over between simulation frames.
	{

		bIsJumpJustPressed = false;
		bShouldToggleFlying = false;
	}
	
	// Does interaction go here? I don't think so.
	// Crouching should me thinks.
}

//============================
// Event Handlers
//============================

void AEpicJam_PlayerPawn::OnMoveTriggered(const FInputActionValue& value)
{
	const FVector MovementVector = value.Get<FVector>();
	CachedMoveInputIntent.X = FMath::Clamp(MovementVector.X, -1.0f, 1.0f);
	CachedMoveInputIntent.Y = FMath::Clamp(MovementVector.Y, -1.0f, 1.0f);
	CachedMoveInputIntent.Z = FMath::Clamp(MovementVector.Z, -1.0f, 1.0f);

	/*bool BoolValue = value.Get<bool>(); // for digital input actions
	float FloatValue = value.Get<float>(); // for Axis1D input actions
	FVector2D Axis2DValue = value.Get<FVector2D>(); // for Axis2D input actions
	FVector VectorValue = value.Get<FVector>(); // for Axis3D input actions*/

	BP_Move(CachedMoveInputIntent);
}

void AEpicJam_PlayerPawn::OnMoveCompleted(const FInputActionValue& value)
{
	CachedMoveInputIntent = FVector::ZeroVector;
	
	BP_Move(CachedMoveInputIntent);
}

void AEpicJam_PlayerPawn::OnLookTriggered(const FInputActionValue& value)
{
	const FVector2D LookVector = value.Get<FVector2D>();
	// TODO: Add the clamps if controller is involved. Leave uncapped for mouse
	CachedLookInput.Yaw = CachedTurnInput.Yaw = LookVector.X; //FMath::Clamp(LookVector.X, -1.0f, 1.0f);
	CachedLookInput.Pitch = CachedTurnInput.Pitch = LookVector.Y; //FMath::Clamp(LookVector.Y, -1.0f, 1.0f);

	BP_Look(LookVector);
}

void AEpicJam_PlayerPawn::OnLookCompleted(const FInputActionValue& value)
{
	CachedLookInput = FRotator::ZeroRotator;
	CachedTurnInput = FRotator::ZeroRotator;

	BP_Look(FVector2D::ZeroVector);
}

void AEpicJam_PlayerPawn::OnJumpStarted(const FInputActionValue& value)
{
	bIsJumpJustPressed = !bIsJumpPressed;
	bIsJumpPressed = true;
	
	BP_Jump(bIsJumpPressed);
}

void AEpicJam_PlayerPawn::OnJumpReleased(const FInputActionValue& value)
{
	bIsJumpPressed = false;
	bIsJumpJustPressed = false;
	
	BP_Jump(bIsJumpPressed);
}

void AEpicJam_PlayerPawn::OnInteract(const FInputActionValue& value)
{
	// Call function to identify interactable object to work with
	// Will there be a tick function that traces for interactables? I could just call cached data from that.
	
	BP_Interact();
}

void AEpicJam_PlayerPawn::OnCrouchTriggered(const FInputActionValue& value)
{
	bIsCrouched = true;

	BP_Crouch(bIsCrouched);
}

void AEpicJam_PlayerPawn::OnCrouchCompleted(const FInputActionValue& value)
{
	bIsCrouched = false;

	BP_Crouch(bIsCrouched);
}
