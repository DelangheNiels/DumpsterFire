// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceObjectSpawner.generated.h"

UCLASS()
class DUMPSTER_FIRE_API ASpaceObjectSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceObjectSpawner();
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<AActor>> m_SpaceObjectsToSpawn{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		bool m_MoveLeftRight = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float m_Width = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float m_Height = 350;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float m_Length = 20;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

		class UBoxComponent* m_pSpawnBox;

		float m_SpawnTimer{ 0.0f };
		float m_SpawnTime{ 3.f };

		

		void SpawnObject();
		

};
