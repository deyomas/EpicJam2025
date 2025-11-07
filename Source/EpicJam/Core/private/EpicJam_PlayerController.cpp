// Header Include
#include "EpicJam_PlayerController.h"

// Engine Includes
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Game Includes
#include "EpicJam.h"

//============================
// Constructor
//============================

AEpicJam_PlayerController::AEpicJam_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// TODO: Do I need any further setup here? Does AActor handle setup for me?
	//InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("EnhancedInputComponent"));
}

void AEpicJam_PlayerController::BeginPlay()
{
	UE_LOG(EpicJam, Log, TEXT("Controller begin play 1"));
	Super::BeginPlay();
	
	//SetupPlayerInputComponent(InputComponent);

	// BP example https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine half way down
	UE_LOG(EpicJam, Log, TEXT("Controller begin play 2"));
	if (ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				UE_LOG(EpicJam, Log, TEXT("Controller applying InputMapping context"));
				inputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
	UE_LOG(EpicJam, Log, TEXT("Controller end Begin Play"));
}

void AEpicJam_PlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UE_LOG(EpicJam, Log, TEXT("Controller possessed pawn"));
}
