// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerGameModeBase.h"


void AEndlessRunnerGameModeBase::BeginPlay()
{
	CreateInitialTiles();
}

void AEndlessRunnerGameModeBase::CreateInitialTiles()
{
	AFloorTile* Tile = AddFloorTile();
	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}

	for (float Val : LaneSwitchValues)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lane Value %f"), Val);
	}

	for (int32 i = 0; i < NumInitialFloorTiles; i++)
	{
		AddFloorTile();
	}
}

AFloorTile* AEndlessRunnerGameModeBase::AddFloorTile()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);
		if (Tile)
		{
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}

	return nullptr;
}