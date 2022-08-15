// Fill out your copyright notice in the Description page of Project Settings.


#include "FireChanger.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireChanger::AFireChanger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Particle
	//m_pRedFireParticleSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Particles/Fire/NS_Fire.NS_Fire'"));
	//m_pBlueFireParticleSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Particles/Fire/NS_BlueFire.NS_BlueFire'"));
}

// Called when the game starts or when spawned
void AFireChanger::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AFireChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pIncinerator)
	{
		if(m_pIncinerator->GetIsActive() == false)
		{
			for (int i = 0; i < m_pFires.Num(); ++i)
			{
				m_pFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pBlueFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pPurpleFlames[i]->GetNiagaraComponent()->DeactivateImmediate();
			}
		}
		else if (m_pIncinerator->GetMultiplier() == 1.0f && m_pFires.Num() != 0)
		{
			for (int i = 0; i < m_pFires.Num(); ++i)
			{
				m_pFires[i]->GetNiagaraComponent()->Activate();
				m_pBlueFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pPurpleFlames[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pRisingBlueFires[i]->GetNiagaraComponent()->DeactivateImmediate();
			}

			m_ActiveFireType = FireType::Normal;

		}
		else if (m_pIncinerator->GetMultiplier() == 1.5f && m_pFires.Num() != 0)
		{
			for (int i = 0; i < m_pFires.Num(); ++i)
			{
				m_pBlueFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pPurpleFlames[i]->GetNiagaraComponent()->Activate();
				m_pRisingBlueFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pRisingFires[i]->GetNiagaraComponent()->DeactivateImmediate();

					
			}

			m_ActiveFireType = FireType::Purple;

			if (m_PlayPurpleRising)
			{
				m_PlayPurpleRising = false;
				m_pIncinerator->SpawnRisingFire();
			}

			
		}
		else if (m_pIncinerator->GetMultiplier() == 2.0f && m_pFires.Num() != 0)
		{
			for (int i = 0; i < m_pFires.Num(); ++i)
			{
				m_pBlueFires[i]->GetNiagaraComponent()->Activate();
				m_pFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pPurpleFlames[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pRisingFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				m_pRisingPurpleFires[i]->GetNiagaraComponent()->DeactivateImmediate();
				
			}
			m_ActiveFireType = FireType::Blue;

			if (m_PlayBlueRising)
			{
				m_PlayBlueRising = false;
				m_pIncinerator->SpawnRisingFire();
			}
		}

	}
}

void AFireChanger::SpawnRisingFlames()
{
	if (m_pIncinerator && m_pIncinerator->GetIsActive())
	{
		switch (m_ActiveFireType)
		{
		case FireType::Normal:
			ActivateRisingFlames(m_pRisingFires);
			break;
		case FireType::Purple:
			ActivateRisingFlames(m_pRisingPurpleFires);
			break;
		case FireType::Blue:
			ActivateRisingFlames(m_pRisingBlueFires);
			break;
		default:
			break;
		}
	}
}

void AFireChanger::DespawnRisingFlames()
{
	if (m_pIncinerator && m_pIncinerator->GetIsActive())
	{
		switch (m_ActiveFireType)
		{
		case FireType::Normal:
			DeactivateRisingFlames(m_pRisingFires);
			break;
		case FireType::Purple:
			DeactivateRisingFlames(m_pRisingPurpleFires);
			break;
		case FireType::Blue:
			DeactivateRisingFlames(m_pRisingBlueFires);
			break;
		default:
			break;
		}
	}
}

void AFireChanger::ActivateRisingFlames(TArray<ANiagaraActor*> fires)
{
	for (size_t i = 0; i < fires.Num(); i++)
	{
		fires[i]->GetNiagaraComponent()->Activate(true);
	}

	if(m_pFireFeedBack != nullptr)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pFireFeedBack, fires[0]->GetActorLocation(), fires[0]->GetActorRotation(), 2, 1);
}

void AFireChanger::DeactivateRisingFlames(TArray<ANiagaraActor*> fires)
{
	for (size_t i = 0; i < fires.Num(); i++)
	{
		fires[i]->GetNiagaraComponent()->DeactivateImmediate();
	}
}

