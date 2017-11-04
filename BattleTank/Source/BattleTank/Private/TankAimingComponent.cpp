// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector WorldSpaceAim, float LaunchSpeed)
{
	if (!Barrel) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	if(UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		WorldSpaceAim,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
		))
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto our_tank_name = GetOwner()->GetName();
		auto from = Barrel->GetComponentLocation().ToString();
		UE_LOG(LogTemp, Warning, TEXT("%s aiming at: %s from %s location. SugLaunchspeed: %f, AimDir: %s"),
			*our_tank_name, *WorldSpaceAim.ToString(), *from, OutLaunchVelocity.Size(), *AimDirection.ToString());

		MoveBarrel(AimDirection);
	} else
	{
		auto our_tank_name = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s has no target"), *our_tank_name);
	}
}

void UTankAimingComponent::MoveBarrel(const FVector AimDirection)
{	
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotation;

	Barrel->Elevate(5);
	//Barrel->SetWorldRotation(AimAsRotator.Quaternion());
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}
