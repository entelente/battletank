// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"
#include "Tank.h"



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

	FVector hit_location;
	if (GetSightRayHitLocation(hit_location))
	{
		GetControlledTank()->AimAt(hit_location);
	} else
    {
        UE_LOG(LogTemp, Warning, TEXT("Hitlocation: *NO HIT*"));
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& out_hit_location) const
{
	int32 viewport_size_x, viewport_size_y;
	GetViewportSize(viewport_size_x, viewport_size_y);
	const auto screen_location = FVector2D(viewport_size_x * CrosshairXLocation, viewport_size_y * CrosshairYLocation);

    FVector look_direction;
    if(GetLookAtDirection(screen_location, look_direction))
    {
        FVector hit_location;
        if(GetLookVectorHitLocation(look_direction, hit_location))
		{
			out_hit_location = hit_location;
            return true;
		}
    }
	out_hit_location = FVector::ZeroVector;
	return false;
}

bool ATankPlayerController::GetLookAtDirection(const FVector2D screen_location, FVector &world_direction) const
{
    FVector world_locaton;
    return DeprojectScreenPositionToWorld(screen_location.X, screen_location.Y, world_locaton, world_direction);
}

bool ATankPlayerController::GetLookVectorHitLocation(const FVector look_direction, FVector & hit_location) const
{
	FHitResult hit_result;
	const auto start_location = PlayerCameraManager->GetCameraLocation();
	const auto end_location = start_location + (look_direction * LineTraceRange);
	if(GetWorld()->LineTraceSingleByChannel(hit_result, start_location, end_location, ECollisionChannel::ECC_Visibility))
	{
		hit_location = hit_result.Location;
		return true;
	}
    hit_location = FVector::ZeroVector;
	return false;	
}


