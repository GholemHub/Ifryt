// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Attack_base.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

UAttack_base::UAttack_base()
{
    PrimaryComponentTick.bCanEverTick = false; // not needed
}

void UAttack_base::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // Create FirePoint once, attach to owner root, apply offset
    FirePoint = NewObject<USceneComponent>(Owner, TEXT("FirePoint"));
    FirePoint->RegisterComponent();
    FirePoint->AttachToComponent(
        Owner->GetRootComponent(),
        FAttachmentTransformRules::KeepRelativeTransform
    );
    FirePoint->SetRelativeLocation(FirePointOffset);

    UE_LOG(LogTemp, Log,
        TEXT("Attack_base: FirePoint created at world loc %s"),
        *FirePoint->GetComponentLocation().ToString());
}

void UAttack_base::Fire()
{
    // ── Guards ────────────────────────────────────────────────
    AActor* Owner = GetOwner();
    if (!Owner || !FirePoint || !ProjectileClass || !GetWorld())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Fire aborted — Owner:%s FirePoint:%s Class:%s World:%s"),
            Owner ? TEXT("OK") : TEXT("NULL"),
            FirePoint ? TEXT("OK") : TEXT("NULL"),
            ProjectileClass ? TEXT("OK") : TEXT("NULL"),
            GetWorld() ? TEXT("OK") : TEXT("NULL"));
        return;
    }

    // ── Compute spawn location & direction ────────────────────
    const FVector SpawnLocation = FirePoint->GetComponentLocation();

    FVector FireDirection;
    if (bAimAtPlayer)
    {
        APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Player)
        {
            FireDirection = (Player->GetActorLocation() - SpawnLocation)
                .GetSafeNormal();
        }
        else
        {
            FireDirection = Owner->GetActorForwardVector(); // fallback
        }
    }
    else
    {
        FireDirection = Owner->GetActorForwardVector();
    }

    const FRotator SpawnRotation = FireDirection.Rotation();

    UE_LOG(LogTemp, Log,
        TEXT("Fire — loc: %s  dir: %s  class: %s"),
        *SpawnLocation.ToString(),
        *FireDirection.ToString(),
        *GetNameSafe(ProjectileClass));

    // ── Spawn ──────────────────────────────────────────────────
    FActorSpawnParameters Params;
    Params.Owner = Owner;
    Params.Instigator = Owner->GetInstigator();
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* Projectile = GetWorld()->SpawnActor<AActor>(
        ProjectileClass, SpawnLocation, SpawnRotation, Params);

    if (!Projectile)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("SpawnActor failed for %s"), *GetNameSafe(ProjectileClass));
        return;
    }

    // ── Apply velocity / impulse ───────────────────────────────
    const FVector LaunchVelocity = FireDirection * ProjectileSpeed;

    // Path A — ProjectileMovementComponent (preferred)
    UProjectileMovementComponent* ProjMove =
        Projectile->FindComponentByClass<UProjectileMovementComponent>();

    if (ProjMove)
    {
        ProjMove->Velocity = LaunchVelocity;
        ProjMove->MaxSpeed = ProjectileSpeed;
        UE_LOG(LogTemp, Log, TEXT("[ProjMove] velocity set: %s"), *LaunchVelocity.ToString());
    }
    // Path B — Physics impulse on StaticMesh (your current BP setup)
    else
    {
        UStaticMeshComponent* Mesh =
            Projectile->FindComponentByClass<UStaticMeshComponent>();

        if (Mesh && Mesh->IsSimulatingPhysics())
        {
            Mesh->AddImpulse(LaunchVelocity, NAME_None, /*bVelChange=*/true);
            UE_LOG(LogTemp, Log, TEXT("[Impulse] applied: %s"), *LaunchVelocity.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Projectile %s has no movement path — enable Simulate Physics on mesh!"),
                *Projectile->GetName());
        }
    }


    UE_LOG(LogTemp, Log,
        TEXT("Spawned: %s  velocity: %s"),
        *Projectile->GetName(), *LaunchVelocity.ToString());
}