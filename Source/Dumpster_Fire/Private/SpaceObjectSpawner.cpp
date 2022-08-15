// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceObjectSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Meteor.h"


// Sets default values
ASpaceObjectSpawner::ASpaceObjectSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pSpawnBox->SetBoxExtent(FVector(m_Width, m_Length, m_Height));
	m_pSpawnBox->SetCollisionProfileName("Trigger");

	m_pSpawnBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpaceObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_SpawnTimer += DeltaTime;

	if (m_SpawnTimer >= m_SpawnTime)
	{
		SpawnObject();
		m_SpawnTimer = 0;
	}

}

void ASpaceObjectSpawner::SpawnObject()
{
	const FVector origin = this->GetActorLocation();
	FVector spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(origin, m_pSpawnBox->GetScaledBoxExtent());

	int i = FMath::RandRange(0, m_SpaceObjectsToSpawn.Num() - 1);
	auto object = GetWorld()->SpawnActor<AActor>(m_SpaceObjectsToSpawn[i], spawnPoint, FRotator(0, 0, 0));

	auto meteorite = Cast<AMeteor>(object);
	meteorite->SetMoveLeftToRight(m_MoveLeftRight);

}

