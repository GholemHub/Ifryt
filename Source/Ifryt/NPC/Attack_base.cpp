// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC/Attack_base.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

UAttack_base::UAttack_base()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAttack_base::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

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
        FireDirection = Player
            ? (Player->GetActorLocation() - SpawnLocation).GetSafeNormal()
            : Owner->GetActorForwardVector();
    }
    else
    {
        FireDirection = Owner->GetActorForwardVector();
    }

    const FRotator SpawnRotation = FireDirection.Rotation();

    // ── Spawn ─────────────────────────────────────────────────
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

    // ── Bind collision → destroy ───────────────────────────────
    if (UPrimitiveComponent* Root =
        Cast<UPrimitiveComponent>(Projectile->GetRootComponent()))
    {
        Root->SetNotifyRigidBodyCollision(true);
    }

    Projectile->OnActorHit.AddDynamic(this, &UAttack_base::OnProjectileHit);

    // ── Configure straight flight ──────────────────────────────
    UProjectileMovementComponent* ProjMove =
        Projectile->FindComponentByClass<UProjectileMovementComponent>();

    if (ProjMove)
    {
        ProjMove->Velocity = FireDirection * ProjectileSpeed;
        ProjMove->InitialSpeed = ProjectileSpeed;
        ProjMove->MaxSpeed = ProjectileSpeed;
        ProjMove->ProjectileGravityScale = 0.f;
        ProjMove->bShouldBounce = false;
        ProjMove->Activate(true);

        UE_LOG(LogTemp, Log,
            TEXT("Fire — vel: %s"), *ProjMove->Velocity.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Projectile %s has no UProjectileMovementComponent!"),
            *Projectile->GetName());
    }
}

void UAttack_base::OnProjectileHit(AActor* SelfActor, AActor* OtherActor,
    FVector NormalImpulse, const FHitResult& Hit)
{
    if (!SelfActor) return;

    if (OtherActor && OtherActor != SelfActor)
    {
        UGameplayStatics::ApplyDamage(
            OtherActor,
            DamageAmount,              // float you define in component
            GetOwner()->GetInstigatorController(),
            SelfActor,
            UDamageType::StaticClass()
        );

        UE_LOG(LogTemp, Warning, TEXT("Projectile Damaged"));
    }

    SelfActor->Destroy();
}