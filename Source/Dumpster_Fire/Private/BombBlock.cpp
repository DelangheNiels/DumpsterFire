// Fill out your copyright notice in the Description page of Project Settings.


#include "BombBlock.h"
#include "Explosion.h"
#include "Components/BoxComponent.h"
#include "../Dumpster_FireCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABombBlock::ABombBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_meshSize = { 95,52,52 };
	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Game/Models/Specials/PROP_Bomb.PROP_Bomb'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

}

// Called when the game starts or when spawned
void ABombBlock::BeginPlay()
{
	Super::BeginPlay();
	float random = FMath::FRandRange(1.f, 1.2f);
	m_pAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), m_pCountdownSound, 1, 1, 0);
	
}

// Called every frame
void ABombBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_ElapsedTime += DeltaTime;
	

	auto p1 = UGameplayStatics::GetPlayerController(GetWorld(), 0);


	if (m_ElapsedTime >= m_ExplosionTimer)
	{
		Explode();
	}

}

void ABombBlock::Explode()
{
	GetWorld()->SpawnActor<AExplosion>(GetActorLocation(), FRotator(0, 0, 0));
	DoControllerRumble();
	if(m_pAudioComponent)
		m_pAudioComponent->Deactivate();

	if(m_pExplosionSound != nullptr)
	{
		float random = FMath::FRandRange(0.0f, 10.f);
		UGameplayStatics::PlaySound2D(GetWorld(), m_pExplosionSound, 10, random);
	}

	Destroy();
}

void ABombBlock::DoControllerRumble()
{

	auto p1 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto p2 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	auto p3 = UGameplayStatics::GetPlayerController(GetWorld(), 2);
	auto p4 = UGameplayStatics::GetPlayerController(GetWorld(), 3);

	if(m_pForceFeedback != nullptr)
	{
		p1->ClientPlayForceFeedback(m_pForceFeedback, false, false, "Explosion");
		p2->ClientPlayForceFeedback(m_pForceFeedback, false, false, "Explosion");
		p3->ClientPlayForceFeedback(m_pForceFeedback, false, false, "Explosion");
		p4->ClientPlayForceFeedback(m_pForceFeedback, false, false, "Explosion");
	}

	if(m_CameraShake != nullptr)
	p1->PlayerCameraManager->StartCameraShake(m_CameraShake);

	
	


}

void ABombBlock::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		if (m_isThrown)
		{
			auto character = Cast<ADumpster_FireCharacter>(OtherActor);

			if (OtherActor != m_pPlayer)
			{
				
				m_pPlayer = nullptr;
				m_pPlayer = nullptr;
				m_isThrown = false;
				Explode();
			}
		}
	}
	else
	{
		if (m_isThrown)
		{
			m_pPlayer = nullptr;
			m_isThrown = false;
		}

	}
}

