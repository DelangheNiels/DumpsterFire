// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Public/InputReceiver.h"
#include "Dumpster_FireGameMode.generated.h"

UCLASS(minimalapi)
class ADumpster_FireGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADumpster_FireGameMode();

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<APawn> m_pInputReceiver {};


protected:
	virtual void BeginPlay() override;

private:
	TArray<AActor*> m_pPlayerStarts{};
	TArray<APlayerController*> m_pPlayerControllers{};
	TArray<APawn*> m_pPlayers{};

	void FindPlayerSpawns();
	void SpawnPlayers();
	void PossessPlayers();

	void SpawnInputReceivers();

	int m_index{0};
	int m_AmountOfPlayersJoined{ 0 };

	bool m_HasSpawned{ false };

};



