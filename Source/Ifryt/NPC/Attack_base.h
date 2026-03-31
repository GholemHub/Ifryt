// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attack_base.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IFRYT_API UAttack_base : public UActorComponent
{
	GENERATED_BODY()
public:
    UAttack_base();

protected:
    virtual void BeginPlay() override;

public:
    /** The class to spawn as a projectile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    TSubclassOf<AActor> ProjectileClass;

    /** Speed applied to projectile via impulse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float ProjectileSpeed = 2000.f;

    /** Offset from owner root where projectile spawns */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    FVector FirePointOffset = FVector(100.f, 0.f, 50.f);

    /** If true, aims toward player. If false, uses owner forward. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    bool bAimAtPlayer = true;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void Fire();

private:
    /** Created once in BeginPlay, attached to owner root */
    UPROPERTY()
    USceneComponent* FirePoint = nullptr;
};