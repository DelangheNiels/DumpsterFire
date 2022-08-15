// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_pMesh->SetSimulatePhysics(false);
	m_pMesh->AttachToComponent(RootComponent, rules);

	//m_pTrashMesh->SetMassOverrideInKg(NAME_None, 15000);


	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (m_pMesh)
	{
		//m_pMesh->SetStaticMesh(m_pMesh.Class);
		m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

}

void AMeteor::SetMoveLeftToRight(bool moveLtR)
{
	m_MoveLeftToRight = moveLtR;
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	m_MovementSpeed = FMath::RandRange(m_MinMovementSpeed, m_MaxMovementSpeed);

	m_UpMovement = FMath::RandRange(m_MinUpMovement, m_MaxUpMovement);

	m_RandomRotation = FMath::RandRange(m_MinRotationSpeed, m_MaxRotationSpeed);
	m_RandomAxis = FMath::RandRange(1, 2);

	float size = FMath::RandRange(m_MinSize, m_MaxSize);
	SetActorScale3D(FVector(size, size, size));
}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_TimeAlive += DeltaTime;

	auto pos = GetActorLocation();
	if(m_MoveLeftToRight)
		pos.Y += m_MovementSpeed * DeltaTime;
	else
		pos.Y -= m_MovementSpeed * DeltaTime;

	pos.Z += m_UpMovement * DeltaTime;
	SetActorLocation(pos);

	auto currentRotation = GetActorRotation();

	if (m_RandomAxis == 1)
	{
		currentRotation += FRotator(currentRotation.Vector().X +  m_RandomRotation * DeltaTime, currentRotation.Vector().Y, currentRotation.Vector().Z);
		
	}

	else if(m_RandomAxis == 2)
	{
		currentRotation += FRotator(currentRotation.Vector().X, currentRotation.Vector().Y + m_RandomRotation * DeltaTime, currentRotation.Vector().Z);
	}

	else
	{
		currentRotation += FRotator(currentRotation.Vector().X, currentRotation.Vector().Y, currentRotation.Vector().Z + m_RandomRotation * DeltaTime);
	}

	SetActorRotation(currentRotation);

	if (m_TimeAlive >= m_MaxAliveTime)
	{
		Destroy();
	}

}

