// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class ABeam;
class APin;
/**
 * 
 */
UCLASS()
class EXOBRIDGE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	UInputMappingContext *GetDefaultMappingContext();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext *InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *MouseClickAction;

	void MouseClick(const FInputActionValue &Value);
	void MouseEndClick(const FInputActionValue &Value);

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem *Subsystem;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSubclassOf<AActor> PlacePinClass;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSubclassOf<AActor> PlaceStrongPinClass;

	UPROPERTY(EditAnywhere, Category = "Input")
TSubclassOf<ABeam> BeamClass;

	UPROPERTY(EditAnywhere, Category = "Input")
TSubclassOf<APin> PinClass;

	AActor *CurrentPin = nullptr;
	bool bIsPlacingPin = false;
};
