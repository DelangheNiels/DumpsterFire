// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalScrap.h"

#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMetalScrap::AMetalScrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Points = 4;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Trash/PROP_Cog2.PROP_Cog2'"));
	m_meshSize = { 200,200,100 };
	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));
	

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		m_pTrashMesh->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));
	}

	
}

// Called when the game starts or when spawned
void AMetalScrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetalScrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

