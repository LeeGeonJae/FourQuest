// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "FQMonsterAIController.h"
#include "FQMonsterDataAsset.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if(AIC)
	{
		AActor* Target = Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
		AFQMonsterBase* Monster = AIC->GetPawn<AFQMonsterBase>();
		if (Monster)
		{
			float Distance = FVector::Distance(Monster->GetActorLocation(), Target->GetActorLocation());
			switch (TargetCondition)
			{
			case ECondition::LessThen:
			{
				if (Distance < Monster->mMonsterDataAsset->mAttackRange)
				{
					Monster->mMonsterState = TargetState;
				}
			}
				break;
			case ECondition::GreaterThen:
			{
				if (Distance > Monster->mMonsterDataAsset->mAttackRange)
				{
					Monster->mMonsterState = TargetState;
				}
			}
				break;
			default:
				break;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
