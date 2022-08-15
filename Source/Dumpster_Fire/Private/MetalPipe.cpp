// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalPipe.h"
#include "Components/BoxComponent.h"

// Sets default values
AMetalPipe::AMetalPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Points = 9;


	m_meshSize = { 25,110,18 };
	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Trash/PROP_Pipe.PROP_Pipe'"));


	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AMetalPipe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetalPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

