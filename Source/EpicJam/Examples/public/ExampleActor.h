#pragma once

// Required Includes
#include "GameFramework/Actor.h"

// Engine Includes
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Plugin Includes
// Module Includes
// Game Includes

// Generated Header
#include "ExampleActor.generated.h"

//============================
// Enums
//============================

//============================
// Structs
//============================

// Forward declarations

// Static variables

UCLASS()
class EPICJAM_API AExampleActor : public AActor
{
	GENERATED_BODY()
	
	//============================
	// Variables
	//============================
public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> AddedComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	//============================
	// Constructor
	//============================
public:
	AExampleActor(const FObjectInitializer& ObjectInitializer);


	//============================
	// API
	//============================
public:


	//============================
	// Inherited
	//============================
public: 
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;


	//============================
	// Internal
	//============================
protected:

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