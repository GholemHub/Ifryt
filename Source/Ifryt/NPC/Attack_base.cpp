// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Attack_base.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UAttack_base::UAttack_base()
{
    PrimaryComponentTick.bCanEverTick = true;

    FirePoint = nullptr; // IMPORTANT
}

void UAttack_base::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    if (!FirePoint)
    {
        FirePoint = NewObject<USceneComponent>(Owner, TEXT("FirePoint"));
        FirePoint->RegisterComponent();

        FirePoint->AttachToComponent(
            Owner->GetRootComponent(),
            FAttachmentTransformRules::KeepRelativeTransform
        );

        FirePoint->SetRelativeLocation(FVector(100, 0, 50));
    }
}


// Called every frame
void UAttack_base::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttack_base::Fire()
{
    if (!ProjectileClass || !FirePoint)
    {
        UE_LOG(LogTemp, Warning, TEXT("FirePoint or ProjectileClass is NULL"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = FirePoint->GetComponentLocation();
    FRotator SpawnRotation;

    if (bUseFirePointRotation)
    {
        SpawnRotation = FirePoint->GetComponentRotation();
    }
    else
    {
        SpawnRotation = GetOwner()->GetActorRotation();
    }

    FActorSpawnParameters Params;
    Params.Owner = GetOwner();
    Params.Instigator = GetOwner()->GetInstigator();

    AActor* Projectile = World->SpawnActor<AActor>(
        ProjectileClass,
        SpawnLocation,
        SpawnRotation,
        Params
    );

    if (!Projectile) return;

    // Try to set velocity if it has ProjectileMovementComponent
    UProjectileMovementComponent* Movement =
        Projectile->FindComponentByClass<UProjectileMovementComponent>();

    if (Movement)
    {
        FVector Direction = SpawnRotation.Vector();
        Movement->Velocity = Direction * ProjectileSpeed;
    }
}