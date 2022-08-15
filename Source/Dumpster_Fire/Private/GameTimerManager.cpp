// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Incinerator.h"

// Sets default values
AGameTimerManager::AGameTimerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}



// Called when the game starts or when spawned
void AGameTimerManager::BeginPlay()
{
	Super::BeginPlay();

	//auto startbutton = UGameplayStatics::GetActorOfClass(GetWorld(), AStartButton::StaticClass());
	//m_pStartButton = Cast<AStartButton>(startbutton);

	m_Run = false;
	m_BlueTeamWon = false;
	m_RedTeamWon = false;
	m_Started = false;
	m_IsGameEnded = false;
	
}

// Called every frame
void AGameTimerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_Run)
	{
		m_ElaspedTime += DeltaTime;

		if (m_ElaspedTime >= 1)
		{
			if (m_Seconds == 0)
			{
				m_Seconds = 59;
				m_Minutes--;
			}

			else
			{
				m_Seconds--;
			}

			m_ElaspedTime = 1 - m_ElaspedTime;

			if (m_Minutes == 0 && m_Seconds == 0)
			{
				m_Run = false;
				m_pStartButton->m_IsGameStarted = false;
				GetWinnerWhenTimeIsOver();
			}
		}
	}

	else
	{
		m_Minutes = 0;
		m_Seconds = 0;
		
	}

	if (m_pStartButton->m_IsGameStarted && m_Run == false && !m_Started)
	{
		m_Minutes = 2;
		m_Seconds = 59;
		m_Run = true;
		m_Started = true;
	}
	
}

bool AGameTimerManager::GetIsGameStarted() const
{
	return m_Run;
}

void AGameTimerManager::EndGame()
{
	m_Run = false;
	m_IsGameEnded = true;
}

void AGameTimerManager::GetWinnerWhenTimeIsOver()
{
	TArray<AActor*> incinerators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIncinerator::StaticClass(), incinerators);
	AIncinerator* inc1{};
	AIncinerator* inc2{};
	for (int i = 0; i < 2; i++)
	{
		auto incinertator = Cast<AIncinerator>(incinerators[i]);
		if (incinertator->m_TeamId == 0)
		{
			inc1 = incinertator;
		}

		else
		{
			inc2 = incinertator;
		}
	}

	if (inc1->GetTotalPoints() >= inc2->GetTotalPoints())
	{
		m_RedTeamWon = true;

		auto p1 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		p1->SetLost(true);

		auto p2 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
		p2->SetLost(true);

		auto p3 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 2));
		p3->SetWon(true);

		auto p4 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 3));
		p4->SetWon(true);
	}

	else
	{
		m_BlueTeamWon = true;

		auto p1 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		p1->SetWon(true);

		auto p2 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
		p2->SetWon(true);

		auto p3 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 2));
		p3->SetLost(true);

		auto p4 = static_cast<ADumpster_FireCharacter*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 3));
		p4->SetLost(true);
	}

	EndGame();
}

