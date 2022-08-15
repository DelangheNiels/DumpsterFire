// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyBox.h"

// Sets default values
ASkyBox::ASkyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };

		m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		m_pStaticMesh->AttachToComponent(RootComponent, rules);
		
	

}

// Called when the game starts or when spawned
void ASkyBox::BeginPlay()
{
	Super::BeginPlay();
	m_pStaticMesh->SetMaterial(0, m_pMaterial);
}

// Called every frame
void ASkyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Rotation += m_RotationSpeed * DeltaTime;
	
	m_pStaticMesh->SetRelativeRotation(FRotator(0,m_Rotation, 0));
}

