#include "EpicJam_GameMode.h"

// Module Includes
#include "EpicJam_GameState.h"
#include "EpicJam_PlayerController.h"
#include "EpicJam_PlayerState.h"

//============================
// Constructor
//============================

AEpicJam_GameMode::AEpicJam_GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AEpicJam_GameState::StaticClass();
	PlayerControllerClass = AEpicJam_PlayerController::StaticClass();
	PlayerStateClass = AEpicJam_PlayerState::StaticClass();
}
