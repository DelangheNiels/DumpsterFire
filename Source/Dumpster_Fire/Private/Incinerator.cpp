// Fill out your copyright notice in the Description page of Project Settings.


#include "Incinerator.h"

#include "BlackHoleBlock.h"
#include "Components/BoxComponent.h"
#include "../BaseTrash.h"
#include "BombBlock.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "FireChanger.h"
#include "JerryCan.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AIncinerator::AIncinerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(160, 50, 120));
	CollisionBox->SetCollisionProfileName("Trigger");
	CollisionBox->SetupAttachment(RootComponent);

}

void AIncinerator::SpawnRisingFire()
{
	
	m_pFireChanger->SpawnRisingFlames();
	m_IsRisingFireSpawned = true;
	m_SpawnRisingTimer = 0;
}

int AIncinerator::GetPointsToWin()
{
	return m_pGameManager->m_PointsToWin;
}

// Called when the game starts or when spawned
void AIncinerator::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AIncinerator::OnOverlapBegin);
	/*auto manager = UGameplayStatics::GetActorOfClass(GetWorld(), AGameTimerManager::StaticClass());
	m_pGameManager = Cast<AGameTimerManager>(manager);*/

	auto spawner = UGameplayStatics::GetActorOfClass(GetWorld(), ATrashSpawner::StaticClass());
	m_pSpawner = Cast<ATrashSpawner>(spawner);

	m_pSmokeParticle->GetNiagaraComponent()->DeactivateImmediate();

}

// Called every frame
void AIncinerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (m_PlaySmoke)
	{
		m_SmokeTime += DeltaTime;
		if (m_SmokeTime >= m_MaxSmokeTime)
		{
			m_SmokeTime = 0;
			m_PlaySmoke = false;
			m_pSmokeParticle->GetNiagaraComponent()->DeactivateImmediate();
		}
	}

	if (m_isBlackHoleActive)
	{
		m_BlackHoleTime += DeltaTime;

		if (m_BlackHoleTime >= m_MaxBlackTime)
		{
			m_BlackHoleTime = 0;
			m_isBlackHoleActive = false;

			if (m_pBlackHoleParticle != nullptr)
				m_pBlackHoleParticle->GetNiagaraComponent()->Deactivate();


		}

	}

	if (m_PlayerHitCount == 1)
	{
		m_ClearTimer += DeltaTime;
		if (m_ClearTimer >= 2)
		{
			m_ClearTimer = 0;
			m_PlayerHitCount = 0;
		}
	}

	if (m_ShowPoints)
	{
		m_ShowPointsTimer += DeltaTime;
		if (m_ShowPointsTimer >= m_ShowPointsTime)
		{
			m_ShowPointsTimer = 0.0f;
			m_ShowPoints = 0;
		}
	}

	if (m_AddPoints)
	{
		if (m_PointsToAdd > 0)
		{
			m_AddPointsTimer += DeltaTime;
			if (m_AddPointsTimer >= m_AddPointsTime)
			{
				++m_TotalScore;
				--m_PointsToAdd;
				m_AddPointsTimer = 0.0f;
			}
		}

		else
		{
			m_AddPoints = false;
			m_AddPointsTimer = 0.0f;
		}
	}


	if (m_TotalScore >= m_pGameManager->m_PointsToWin && m_pGameManager->GetIsGameStarted())
	{
		if (m_TeamId == 0)
		{
			m_pGameManager->m_RedTeamWon = true;

			auto p1 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			p1->SetLost(true);
		
			//p1->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			auto p2 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
			p2->SetLost(true);

			//p2->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			auto p3 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 2));
			p3->SetWon(true);
			
			//p3->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			auto p4 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 3));
			
			//p4->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			p4->SetWon(true);
		}

		else
		{
			m_pGameManager->m_BlueTeamWon = true;

			auto p1 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

			if (p1 != nullptr)
			{
				p1->SetWon(true);

				//p1->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}


			auto p2 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));

			if (p2 != nullptr)
			{
				p2->SetWon(true);
			
				//p2->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}


			auto p3 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 2));
			if (p3 != nullptr)
			{
				p3->SetLost(true);
				
				//p3->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}



			auto p4 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 3));
			if (p4 != nullptr)
			{
				p4->SetLost(true);
				
				//p4->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}





		}

		m_pGameManager->EndGame();
	}

	if (m_IsActive == false)
	{
		m_ElapsedInactiveTime += DeltaTime;

		if (m_ElapsedInactiveTime >= m_MaxInactiveTime)
		{
			m_IsActive = true;
			m_ElapsedInactiveTime = 0;

		}
	}

	if (m_IsRisingFireSpawned)
	{
		m_SpawnRisingTimer += DeltaTime;
		if (m_SpawnRisingTimer >= m_RisingFlameTime)
		{
			m_SpawnRisingTimer = 0;
			m_IsRisingFireSpawned = false;
			m_pFireChanger->DespawnRisingFlames();
		}
	}

}

void AIncinerator::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_IsActive)
	{

		m_pSmokeParticle->GetNiagaraComponent()->Activate(true);
		m_PlaySmoke = true;

		if (OtherActor->IsA(ABlackHoleBlock::StaticClass()))
		{
			auto pBlackHole = Cast<ABlackHoleBlock>(OtherActor);

			auto incinerator = Cast<AIncinerator>(this);

			if (incinerator != nullptr)
			{
				pBlackHole->PullBlocksTowardsIncinerator(incinerator);
				m_isBlackHoleActive = true;
				if (m_pBlackHoleParticle != nullptr)
					m_pBlackHoleParticle->GetNiagaraComponent()->Activate();

				m_pFireChanger->SpawnRisingFlames();
				m_IsRisingFireSpawned = true;

			}

		}

		else if (OtherActor->IsA(ABombBlock::StaticClass()))
		{
			auto pBomb = Cast<ABombBlock>(OtherActor);
			m_IsActive = false;

			pBomb->Explode();
			m_index = 0;


		}

		else if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
		{
			m_PlayerHitCount++;
			if (m_PlayerHitCount >= 2)
			{
				auto p = Cast<ADumpster_FireCharacter>(OtherActor);
				p->PlayerDied();
				m_PlayerHitCount = 0;

				m_pFireChanger->SpawnRisingFlames();
				m_IsRisingFireSpawned = true;
			}


		}
		else if (OtherActor->IsA(AJerryCan::StaticClass()))
		{
			auto jerrycan = Cast<AJerryCan>(OtherActor);
			if (m_index < m_scoreMultipliers.size() - 1)
			{
				++m_index;
			}

			m_pFireChanger->SpawnRisingFlames();
			m_IsRisingFireSpawned = true;

			OtherActor->Destroy();
		}
		else if (OtherActor->IsA(ABaseTrash::StaticClass()))
		{
			auto pTrashObj = Cast<ABaseTrash>(OtherActor);
			if (m_pGameManager->GetIsGameStarted())
			{
				m_PointsToAdd += static_cast<int>(pTrashObj->GetPoints() * m_scoreMultipliers[m_index]);
				m_PointsToShow = pTrashObj->GetPoints() * m_scoreMultipliers[m_index];
				m_AddPoints = true;
				m_ShowPoints = true;
			}

			m_pFireChanger->SpawnRisingFlames();
			m_IsRisingFireSpawned = true;

			OtherActor->Destroy();
		}

	}
	else if (OtherActor->IsA(AJerryCan::StaticClass()))
	{
		m_IsActive = true;
		OtherActor->Destroy();
	}

	else
		OtherActor->Destroy();



}

int AIncinerator::GetTotalPoints() const
{
	return m_TotalScore;

}

float AIncinerator::GetMultiplier() const
{
	return m_scoreMultipliers[m_index];
}

bool AIncinerator::GetIsActive() const
{
	return m_IsActive;
}


