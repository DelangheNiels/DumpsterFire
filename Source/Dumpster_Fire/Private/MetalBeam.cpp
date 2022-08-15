// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalBeam.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"


// Sets default values
AMetalBeam::AMetalBeam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Points = 7;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Trash/PROP_MetalBeam.PROP_MetalBeam'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	m_meshSize = { 13,45,19 };
	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));
}

// Called when the game starts or when spawned
void AMetalBeam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetalBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

