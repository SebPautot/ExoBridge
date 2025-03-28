// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pin.generated.h"

UCLASS()
class EXOBRIDGE_API APin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector ForceSum = FVector(0, 0, 0);
	float MassSum = 0;
	int ConnectionSum = 0;

	UPROPERTY(EditAnywhere)
	bool IsFixed = false;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	UStaticMeshComponent* MeshComponent;
};
