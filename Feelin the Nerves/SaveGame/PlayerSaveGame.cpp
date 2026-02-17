// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGame/PlayerSaveGame.h"

int UPlayerSaveGame::GetPlayerMaxPlayedSequences()
{
	return savedMaxPlayedSequences;
}

int UPlayerSaveGame::GetPlayerMaxCorrectSequences()
{
	return savedMaxCorrectSequences;
}

int UPlayerSaveGame::GetPlayerCurrentCompletedSequences()
{
	return savedCurrentCompletedSequences;
}

void UPlayerSaveGame::SetPlayerMaxPlayedSequences(int value_)
{
	savedMaxPlayedSequences = value_;
}

void UPlayerSaveGame::SetPlayerMaxCorrectSequences(int value_)
{
	savedMaxCorrectSequences = value_;
}

void UPlayerSaveGame::SetPlayerCurrentCompletedSequences(int value_)
{
	savedCurrentCompletedSequences = value_;
}

TArray<int> UPlayerSaveGame::GetSavedRandomNerves()
{
	return savedRandomNerves;
}

int UPlayerSaveGame::GetSavedRandomNerveAtReferenceIndex(int& i)
{
	return !savedRandomNerves.IsEmpty() ? savedRandomNerves[i] : 0;
}

void UPlayerSaveGame::SetSavedRandomNerves(int index_)
{
	savedRandomNerves.Add(index_);
}

void UPlayerSaveGame::EmptySavedRandomNerves()
{
	if (!savedRandomNerves.IsEmpty()) savedRandomNerves.Empty();
}