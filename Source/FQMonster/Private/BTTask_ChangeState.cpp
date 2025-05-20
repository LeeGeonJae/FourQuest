// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState.h"
#include "FQMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AFQMonsterBase* Monster = AIC->GetPawn<AFQMonsterBase>();
		if (Monster)
		{
			Monster->mMonsterState = TargetState;
			AIC->GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), (uint8)TargetState);
		}
	}
	return EBTNodeResult::Succeeded;
}
