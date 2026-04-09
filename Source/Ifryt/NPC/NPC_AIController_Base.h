// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "NPC_AIController_Base.generated.h"

UCLASS()
class IFRYT_API ANPC_AIController_Base : public AAIController
{
	GENERATED_BODY()

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	uint8 TeamID = 1;
};
