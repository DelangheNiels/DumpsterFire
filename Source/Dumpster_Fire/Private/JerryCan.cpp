// Fill out your copyright notice in the Description page of Project Settings.


#include "JerryCan.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

AJerryCan::AJerryCan()
{
	m_meshSize = { 70,50,80 };

	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));
	m_pCollisionBox->SetRelativeLocation({ 0,0,75 });
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Specials/PROP_Gasoline.PROP_Gasoline'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

void AJerryCan::BeginPlay()
{
	Super::BeginPlay();
}
