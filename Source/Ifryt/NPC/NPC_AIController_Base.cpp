#include "NPC/NPC_AIController_Base.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

FGenericTeamId ANPC_AIController_Base::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

ETeamAttitude::Type ANPC_AIController_Base::GetTeamAttitudeTowards(const AActor & Other) const
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
