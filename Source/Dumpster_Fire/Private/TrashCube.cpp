// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashCube.h"

// Sets default values
ATrashCube::ATrashCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Trash/PROP_CompressedCube.PROP_CompressedCube'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		m_pTrashMesh->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));
	}
}

// Called when the game starts or when spawned
void ATrashCube::BeginPlay()
{
	Super::BeginPlay();
	m_Points = 3;
	
}

// Called every frame
void ATrashCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

