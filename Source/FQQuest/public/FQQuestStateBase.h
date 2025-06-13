#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FQQuestStateBase.generated.h"

UCLASS()
class FQQUEST_API UFQQuestStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFQQuestStateBase();

	virtual void StateEnter() PURE_VIRTUAL(UFQQuestStateBase::StateEnter, );
	virtual void StateUpdate(float DeltaTime) PURE_VIRTUAL(UFQQuestStateBase::StateUpdate, );
	virtual void StateExit() PURE_VIRTUAL(UFQQuestStateBase::StateExit, );

	FORCEINLINE void SetOwnerQuestObject(class AFQQuestBase* OwnerQuestObject) { mOwnerQuestObject = OwnerQuestObject; }

private:
	UPROPERTY()
	TObjectPtr<class AFQQuestBase> mOwnerQuestObject;
};
