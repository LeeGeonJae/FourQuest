// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMonsterAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AFQMonsterAIController::AFQMonsterAIController()
{
	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    mSightConfig->SightRadius = 1000.0f;
    mSightConfig->LoseSightRadius = 1200.0f;
    mSightConfig->PeripheralVisionAngleDegrees = 90.0f;
    mSightConfig->SetMaxAge(5.0f);
    mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
    mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    mSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    
    mAIPerception->ConfigureSense(*mSightConfig);
    mAIPerception->SetDominantSense(mSightConfig->GetSenseImplementation());

    mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AFQMonsterAIController::OnTargetPerceptionUpdated);

    mBehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("Behavior Tree"));
    
    SetGenericTeamId(FGenericTeamId(1));
}

void AFQMonsterAIController::SetTargetActor(AActor* Actor)
{
    GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
    ChangeState(EMonsterState::Chase);
}

void AFQMonsterAIController::ChangeState(EMonsterState State)
{
    //ai컨트롤러에서 블랙보드와 몬스터의 상태를 다 바꿔줌
    GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), (uint8)State);
    AFQMonsterBase* Monster = GetPawn<AFQMonsterBase>();
    if (Monster)
    {
        Monster->mMonsterState = State;
    }
}

void AFQMonsterAIController::ChangeTargetActor(AActor* Actor)
{
    AFQMonsterBase* Monster = Cast<AFQMonsterBase>(GetPawn());
    if (Monster)
    {
        Monster->mTargetActor = Actor;
        GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
    }
}

void AFQMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

}

void AFQMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(mBehaviorTree)
    {
        UBlackboardComponent* RawBlackboard = Blackboard.Get();
        UseBlackboard(mBehaviorTree->BlackboardAsset, RawBlackboard);
        Blackboard = RawBlackboard;
        GetBlackboardComponent()->SetValueAsVector(TEXT("OriginLocation"), InPawn->GetActorLocation());

        RunBehaviorTree(mBehaviorTree);
    }
}

void AFQMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor->Tags.Contains(FName("Player")))
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            if (!GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
            {
                if (!Actor->IsA(AFQMonsterBase::StaticClass()))
                {
                    // 감지 성공 시
                    AFQMonsterBase* Monster = Cast<AFQMonsterBase>(GetPawn());
                    Monster->ManagerSetTargetActor(Actor);
                }
            }
        }
        if (Stimulus.Type == UAISense_Damage::GetSenseID(UAISense_Damage::StaticClass()))
        {
            AFQMonsterBase* Monster = Cast<AFQMonsterBase>(GetPawn());
            if(Monster)
            {
                UE_LOG(LogTemp,Warning,TEXT("Damage Config"))
                ChangeState(EMonsterState::Hit);
                Monster->Hit();
                Monster->mTargetActor = Actor;
                GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
            }
        }
    }
}
