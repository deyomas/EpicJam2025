#include "ExampleActor.h"

AExampleActor::AExampleActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AddedComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	AddedComponent->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetStaticMesh();
	AddedComponent->SetupAttachment(RootComponent);
}

//============================
// Inherited
//============================

void AExampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExampleActor::BeginPlay()
{
	Super::BeginPlay();
}
