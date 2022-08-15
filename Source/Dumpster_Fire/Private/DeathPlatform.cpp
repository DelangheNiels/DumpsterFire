// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathPlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADeathPlatform::ADeathPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trash Mesh"));
	m_pMesh->AttachToComponent(RootComponent, rules);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

}

UStaticMeshComponent* ADeathPlatform::GetMesh()
{
	return m_pMesh;
}

// Called when the game starts or when spawned
void ADeathPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(pos);
	m_pMesh->SetSimulatePhysics(false);
	m_pMesh->SetEnableGravity(false);

	m_pMesh->SetWorldScale3D(FVector(20, 20, 1));
	
}

// Called every frame
void ADeathPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

