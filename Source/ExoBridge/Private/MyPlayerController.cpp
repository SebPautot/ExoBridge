// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Pin.h"
#include "Beam.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
        UE_LOG(LogTemp, Warning, TEXT("subsystem loaded"));
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        UE_LOG(LogTemp, Warning, TEXT("input function loaded"));
        EnhancedInputComponent->BindAction(MouseClickAction, ETriggerEvent::Started, this, &AMyPlayerController::MouseClick);
        EnhancedInputComponent->BindAction(MouseClickAction, ETriggerEvent::Completed, this, &AMyPlayerController::MouseEndClick);
    }
}

UInputMappingContext *AMyPlayerController::GetDefaultMappingContext()
{
    return InputMappingContext;
}

void AMyPlayerController::MouseClick(const FInputActionValue &Value)
{

    if(PlacePinClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlacePinClass is null"));
        return;
    }

    if(PlaceStrongPinClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlaceStrongPinClass is null"));
        return;
    }

    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, CursorHit);
    

    if(CursorHit.GetActor() == nullptr)
        return;

    UE_LOG(LogTemp, Warning, TEXT("hit result: %s"), *CursorHit.ImpactPoint.ToString());

    UE_LOG(LogTemp, Warning, TEXT("hit actor: %s"), *CursorHit.GetActor()->GetName());

    if(Cast<APin>(CursorHit.GetActor()))
    {
        UE_LOG(LogTemp, Warning, TEXT("hit pin"));

        CurrentPin = CursorHit.GetActor();
        bIsPlacingPin = true;
    }
    else if(CursorHit.GetActor()->GetClass() == PlacePinClass || CursorHit.GetActor()->GetClass() == PlaceStrongPinClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("hit not pin"));
        
        CurrentPin = GetWorld()->SpawnActor<APin>(PinClass, CursorHit.ImpactPoint, FRotator(0, 0, 0));

        if(CursorHit.GetActor()->GetClass() == PlaceStrongPinClass)
        {
            Cast<APin>(CurrentPin)->IsFixed = true;
        }

        if(CurrentPin != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("spawned pin"));
            bIsPlacingPin = true;
        }
    }
}

void AMyPlayerController::MouseEndClick(const FInputActionValue &Value)
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, CursorHit);
    

    if(CursorHit.GetActor() == nullptr || !bIsPlacingPin){
        bIsPlacingPin = false;
        CurrentPin = nullptr; // can't place pin
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("hit result: %s"), *CursorHit.ImpactPoint.ToString());

    if(Cast<APin>(CursorHit.GetActor()))
    {
        ABeam* Beam = GetWorld()->SpawnActor<ABeam>(BeamClass, CursorHit.ImpactPoint, FRotator(0, 0, 0));
        if(Beam != nullptr)
        {
            Beam->StartPin = Cast<APin>(CurrentPin);
            Beam->EndPin = Cast<APin>(CursorHit.GetActor());
        }
    }
    else if(CursorHit.GetActor()->GetClass() == PlacePinClass || CursorHit.GetActor()->GetClass() == PlaceStrongPinClass)
    {
        ABeam* Beam = GetWorld()->SpawnActor<ABeam>(BeamClass, CursorHit.ImpactPoint, FRotator(0, 0, 0));
        if(Beam != nullptr)
        {
            Beam->StartPin = Cast<APin>(CurrentPin);
            Beam->EndPin = GetWorld()->SpawnActor<APin>(PinClass, CursorHit.ImpactPoint, FRotator(0, 0, 0));

            if(CursorHit.GetActor()->GetClass() == PlaceStrongPinClass)
            {
                Beam->EndPin->IsFixed = true;
            }
        }
    }
}