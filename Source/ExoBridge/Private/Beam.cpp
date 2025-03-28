// Fill out your copyright notice in the Description page of Project Settings.

#include "Beam.h"
#include "Pin.h"

// Sets default values
ABeam::ABeam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = MeshComponent;
	// set the mesh to a cylinder
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(CylinderMesh.Object);
	}
}

// Called when the game starts or when spawned
void ABeam::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StartPin == nullptr || EndPin == nullptr)
	{
		Destroy();
		return;
	}
	else if (!bIsLengthSet)
	{
		BeamLength = FVector::Dist(StartPin->GetActorLocation(), EndPin->GetActorLocation());
		bIsLengthSet = true;
	}

	CurrentLength = FVector::Dist(StartPin->GetActorLocation(), EndPin->GetActorLocation());
	FVector CurrentVector = EndPin->GetActorLocation() - StartPin->GetActorLocation();
	CurrentVector.Normalize();
	FVector Gravity = FVector(0, 0, -980 * Mass);
	Tension = Stiffness * (CurrentLength - BeamLength);
	if (Tension > Resistance && bIsBreakable)
	{
		// Destroy this beam
		Destroy();
		return;
	}

	StartPin->MassSum += Mass;
	StartPin->ConnectionSum += 1;

	EndPin->MassSum += Mass;
	EndPin->ConnectionSum += 1;

	StartPin->ForceSum += ((CurrentVector * Tension) + Gravity) / 2;
	EndPin->ForceSum += ((CurrentVector * Tension * -1) + Gravity) / 2;

	// Set the location of the beam
	SetActorLocation((StartPin->GetActorLocation() + EndPin->GetActorLocation()) / 2);
	SetActorRotation(CurrentVector.Rotation() + FRotator(90, 0, 0));
	SetActorScale3D(FVector(0.1, 0.1, (CurrentLength) / 100 - 2));
}
