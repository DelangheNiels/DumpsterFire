// Fill out your copyright notice in the Description page of Project Settings.


#include "Wheel.h"

#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AWheel::AWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Points = 5;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Trash/PROP_Wheel.PROP_Wheel'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	m_meshSize = { 85, 85 , 30 };
	m_pCollisionBox->SetBoxExtent(m_meshSize);
}

// Called when the game starts or when spawned
void AWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

