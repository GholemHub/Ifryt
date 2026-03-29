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
	// Sets default values for this component's properties
	UAttack_base();

	UPROPERTY(VisibleAnywhere, Category = Fire)
	USceneComponent* FirePoint;

	UPROPERTY(EditAnywhere, Category = Fire)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Fire)
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = Fire)
	bool bUseFirePointRotation = true;

	UFUNCTION(BlueprintCallable)
	void Fire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
