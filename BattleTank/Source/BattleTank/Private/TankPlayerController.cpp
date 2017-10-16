// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"


ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not posessing a tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller posessing: %s"), *(ControlledTank->GetName()))
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitlocation: %s"), *HitLocation.ToString());
	} else
    {
        UE_LOG(LogTemp, Warning, TEXT("Hitlocation: *NO HIT*"));
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

    FVector LookDirection;
    if(GetLookAtDirection(ScreenLocation, LookDirection))
    {
        FVector HitLocation;
        if(GetLookVectorHitLocation(LookDirection, HitLocation))
		{
			OutHitLocation = HitLocation;
            return true;
		}
    }
	OutHitLocation = FVector::ZeroVector;
	return false;
}

bool ATankPlayerController::GetLookAtDirection(const FVector2D ScreenLocation, FVector &WorldDirection) const
{
    FVector WorldLocaton;
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocaton, WorldDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
    HitLocation = FVector::ZeroVector;
	return false;
}


