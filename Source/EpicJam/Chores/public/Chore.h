#pragma once

// Generated include
#include "Chore.generated.h"

UENUM(BlueprintType)
enum class EChoreLocation : uint8
{
	Kitchen,
	LivingRoom,
	FrontYard,
	BackYard,
	Hall,
	Bedroom,
	Bathroom1,
	Bathroom2,
	Garage
};

//============================
// Structs
//============================

USTRUCT(BlueprintType)
struct FChore
{
	GENERATED_BODY()

	//============================
	// Variables
	//============================

	UPROPERTY(BlueprintReadWrite, Category="Chores")
	FName Title;

	UPROPERTY(BlueprintReadWrite, Category="Chores")
	FName Description;
	
	UPROPERTY(BlueprintReadWrite, Category="Chores")
	EChoreLocation Location;

	// Make an enum? Perhaps integrate this into a TMap for different archetypes?
	// Should difficulty be broken into physical and mental? 
	UPROPERTY(BlueprintReadWrite, Category="Chores")
	int Difficulty;

	// A rough metric for how long chores take.
	// TODO: Use an enum? Instant / quick / medium / long ?
	UPROPERTY(BlueprintReadWrite, Category="Chores")
	float TimeToComplete;

	// Whether a task can be partially completed then returned to.
	UPROPERTY(BlueprintReadWrite, Category="Chores")
	bool bIsProgressive;

	// TODO: Use an enum.
	// 1 = whenever. 5 = NOW.
	// TODO: Can urgency be dynamic depending on time constraints? Like make dinner by 7 or else it gets urgent
	UPROPERTY(BlueprintReadWrite, Category="Chores")
	int Urgency;
	
	
	// Will chores be responsible for knowing how much impact they have on characters? Who will be responsible for this?
	// I think the chore subsystem or player relation subsystem should track this info. 
};