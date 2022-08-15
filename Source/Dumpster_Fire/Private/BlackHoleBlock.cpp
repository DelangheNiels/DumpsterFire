// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleBlock.h"

#include "Implosion.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABlackHoleBlock::ABlackHoleBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	m_meshSize = { 70,50,70 };

	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));
	m_pCollisionBox->SetRelativeLocation({ 0,0,-50});
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Specials/PROP_BlackHoleBomb.PROP_BlackHoleBomb'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	
}

void ABlackHoleBlock::PullBlocksTowardsIncinerator(AIncinerator* incinerator)
{
	m_pTrashMesh->SetVisibility(false);
	m_pTrashMesh->SetSimulatePhysics(false);
	m_pTrashMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	if (incinerator != nullptr)
	{
		m_pIncinerator = incinerator;

		if(m_pBlackHoleSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pBlackHoleSound, incinerator->GetActorLocation(), incinerator->GetActorRotation(), 2, 1);
	}



	m_canPulltowardIncinerator = true;

}

void ABlackHoleBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_canPulltowardIncinerator && m_pIncinerator != nullptr)
	{
		UBoxComponent* MeshComp = Cast<UBoxComponent>(m_pIncinerator->GetRootComponent());


		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Impulse Begins"));
		GetWorld()->SpawnActor<AImplosion>(m_pIncinerator->GetActorLocation(), FRotator(0, 0, 0));
		
		m_canPulltowardIncinerator = false;
	
		m_pIncinerator = nullptr;
		Destroy();
	}
	

	
}

void ABlackHoleBlock::BeginPlay()
{
	Super::BeginPlay();
	
	
}

