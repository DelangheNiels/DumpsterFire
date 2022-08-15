// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTimerManager.h"
#include "TrashSpawner.generated.h"

UCLASS()
class DUMPSTER_FIRE_API ATrashSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float m_TimeToSpawnTrash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float m_TimeToSpawnSpecial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning")
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<ABaseTrash>> m_TrashToSpawn{};

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<ABaseTrash>> m_SpecialsToSpawn{};

private:

	float m_TotalElapsedTime;
	float m_TotalElapsedTimeSpecials;

	AGameTimerManager* m_pGameManager;

	void SpawnTrash();
	void SpawnSpecial();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
