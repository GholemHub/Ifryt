// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Ifryt_DetourCrowdAIController.generated.h"

/**
 * 
 */
UCLASS()
class IFRYT_API AIfryt_DetourCrowdAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	uint8 TeamID = 1;
};
