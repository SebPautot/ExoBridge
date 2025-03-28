// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Beam.generated.h"

class APin;

UCLASS()
class EXOBRIDGE_API ABeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APin> StartPin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APin> EndPin;
	float BeamLength = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stiffness = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Resistance = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Tension = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBreakable = true;
private:
	float CurrentLength = 0.0f;
	UStaticMeshComponent* MeshComponent;
	bool bIsLengthSet = false;
};
