// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TrashCube.h"
#include "TrashBag.h"
#include "Wheel.h"
#include "MetalPipe.h"
#include "MetalScrap.h"
#include "Engine/World.h"
#include "BlackHoleBlock.h"
#include "BombBlock.h"
#include "JerryCan.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// Sets default values
ATrashSpawner::ATrashSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_TimeToSpawnTrash = 5.0f;
	m_TotalElapsedTime = 4.0f;

	m_TimeToSpawnSpecial = 20.0f;
	m_TotalElapsedTimeSpecials = 0.f;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(800, 600, 10));
	CollisionBox->SetCollisionProfileName("Trigger");

	CollisionBox->SetupAttachment(RootComponent);

}

void ATrashSpawner::SpawnTrash()
{
	const FVector origin = this->GetActorLocation();
	FVector spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(origin, CollisionBox->GetScaledBoxExtent());

	int i = FMath::RandRange(0, m_TrashToSpawn.Num()-1);
	GetWorld()->SpawnActor<AActor>(m_TrashToSpawn[i], spawnPoint, FRotator(0,0,0));

}

void ATrashSpawner::SpawnSpecial()
{
	const FVector origin = this->GetActorLocation();
	FVector spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(origin, CollisionBox->GetScaledBoxExtent());

	int i = FMath::RandRange(0, m_SpecialsToSpawn.Num() - 1);
	GetWorld()->SpawnActor<AActor>(m_SpecialsToSpawn[i], spawnPoint, FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void ATrashSpawner::BeginPlay()
{
	Super::BeginPlay();
	auto manager = UGameplayStatics::GetActorOfClass(GetWorld(), AGameTimerManager::StaticClass());
	m_pGameManager = Cast<AGameTimerManager>(manager);
}

// Called every frame
void ATrashSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pGameManager->GetIsGameStarted())
	{
		m_TotalElapsedTime += DeltaTime;
		m_TotalElapsedTimeSpecials += DeltaTime;

		if (m_TotalElapsedTime >= m_TimeToSpawnTrash)
		{

			SpawnTrash();
			//SpawnTrash();
			m_TotalElapsedTime = 0;
		}

		if (m_TotalElapsedTimeSpecials >= m_TimeToSpawnSpecial)
		{
			SpawnSpecial();
			m_TotalElapsedTimeSpecials = 0;
		}
	}
	

}

