// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMonsterKillQuestDataAsset.h"

UFQMonsterKillQuestDataAsset::UFQMonsterKillQuestDataAsset()
{
	mQuestClear = false;

	mDescription = TEXT("");

	mMonsterType = EQuestMonsterType::None;
	mMonsterKillNumber = 0;
}
