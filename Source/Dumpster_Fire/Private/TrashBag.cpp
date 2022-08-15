// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashBag.h"

#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATrashBag::ATrashBag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Points = 5;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Trash/PROP_TrashBag.PROP_TrashBag'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	m_pCollisionBox->SetBoxExtent({ 35,35,52 });
}

// Called when the game starts or when spawned
void ATrashBag::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATrashBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

