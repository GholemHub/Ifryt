// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Ifryt_DetourCrowdAIController.h"
#include "Ifryt_DetourCrowdAIController.h"



FGenericTeamId AIfryt_DetourCrowdAIController::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

ETeamAttitude::Type AIfryt_DetourCrowdAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);

	if (!OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}

	const IGenericTeamAgentInterface* OtherTeamAgent =
		Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());

	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	if (OtherTeamAgent->GetGenericTeamId() == GetGenericTeamId())
	{
		return ETeamAttitude::Friendly;
	}

	return ETeamAttitude::Hostile;
}
