// Fill out your copyright notice in the Description page of Project Settings.


#include "IncineratorProgressBar.h"

// Sets default values
AIncineratorProgressBar::AIncineratorProgressBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_pMesh->SetSimulatePhysics(true);
	m_pMesh->AttachToComponent(RootComponent, rules);

}

// Called when the game starts or when spawned
void AIncineratorProgressBar::BeginPlay()
{
	Super::BeginPlay();

	DisableComponentsSimulatePhysics();
	
	if (m_pMesh && m_pMaterial)
	{
		m_pMesh->SetMaterial(0,m_pMaterial);

	}

	if (m_pIncinerator)
	{
		m_ScalePercentage = m_MaxScale / float(m_pIncinerator->GetPointsToWin());
	}

}

// Called every frame
void AIncineratorProgressBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float scale = m_pIncinerator->GetTotalPoints() * m_ScalePercentage;

	if (scale > m_MaxScale)
		scale = m_MaxScale;

	SetActorScale3D(FVector(0.5, 0.3, scale));

	
}

