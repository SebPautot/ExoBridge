// Fill out your copyright notice in the Description page of Project Settings.


#include "Pin.h"

// Sets default values
APin::APin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = MeshComponent;
	//set the mesh to a sphere
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMesh.Object);
	}

	// set simulation mode to simulate physics
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(true);
	//mesh shouldn't collide with anything
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// mesh should not block anything else
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// mesh should still block ECC_GameTraceChannel1
	MeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	//lock z axis
	MeshComponent->SetConstraintMode(EDOFMode::YZPlane);
}

// Called when the game starts or when spawned
void APin::BeginPlay()
{
	Super::BeginPlay();
	
	//Set the mass of the pin
	MeshComponent->SetMassOverrideInKg(NAME_None, 1.0f, true);
}

// Called every frame
void APin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsFixed){
		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetEnableGravity(false);
		return;
	}else{
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetEnableGravity(false);
	}

	// note force is useful each tick but mass not as much, need to be in begin play instead, but if new connections are made we would need an event but we dont have time for this
	// i should stfu
	//Apply force sum
	if(MassSum <= 0)
		MassSum = 1000.f; // not connected to anything, so we will just set it to a default value
	MeshComponent->SetMassOverrideInKg(NAME_None, MassSum, true);

	//remove jerk
	ForceSum -= (ForceSum / MassSum) * DeltaTime;

	MeshComponent->AddForce(ForceSum / MassSum, NAME_None, true);

	
	//Reset the sums
	ForceSum = FVector(0, 0, 0);
	MassSum = 0;
	ConnectionSum = 0;

	
}

