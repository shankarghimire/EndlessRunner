// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FloorTile.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
		int32 NumInitialFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		FTransform NextSpawnPoint;

	//UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	//	TArray<float>LaneSwitchValues;


	UFUNCTION(BlueprintCallable)
		void CreateInitialTiles();

	UFUNCTION(BlueprintCallable)
		void AddFloorTile();


protected:
	virtual void BeginPlay() override;


};
