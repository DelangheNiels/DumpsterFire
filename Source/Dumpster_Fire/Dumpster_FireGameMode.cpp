// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dumpster_FireGameMode.h"

#include <string>

#include "Dumpster_FireCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Dumpster_FireCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADumpster_FireGameMode::ADumpster_FireGameMode()
{

}

void ADumpster_FireGameMode::BeginPlay()
{
	if (!m_HasSpawned)
	{
		FindPlayerSpawns();
		SpawnInputReceivers();
	}
	//SpawnPlayers();
}

void ADumpster_FireGameMode::FindPlayerSpawns()
{
	TArray<AActor*> FoundPlayerStarts{};
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), APlayerStart::StaticClass(), m_pPlayerStarts);

}

//void ADumpster_FireGameMode::SpawnPlayers()
//{
//	for (int i = 0; i < m_pPlayerStarts.Num(); i++)
//	{
//		m_index = i;
//		auto playerStart = m_pPlayerStarts[m_index];
//		auto PlayerCast = Cast<APlayerStart>(playerStart);
//		auto playerTag = PlayerCast->PlayerStartTag;
//
//		for (int j = 0; j < m_pPlayerStarts.Num(); j++)
//		{
//			if (FString::FromInt(j) == playerTag.ToString())
//			{
//				FVector spawnPoint = playerStart->GetActorLocation();
//
//				FActorSpawnParameters SpawnParams;
//				SpawnParams.Owner = this;
//				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//				if (m_index == 0)
//				{
//					auto* player = GetWorld()->SpawnActor<APawn>(m_Players[m_index], spawnPoint, FRotator(0, 0, 0), FActorSpawnParameters{});
//					auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//					controller->Possess(player);
//					ADumpster_FireCharacter* character = Cast<ADumpster_FireCharacter>(player);
//					character->m_Index = 0;
//				
//				}
//				else
//				{
//					auto* player = GetWorld()->SpawnActor<APawn>(m_Players[j], spawnPoint, FRotator(0, 0, 0), FActorSpawnParameters{});
//					auto c = UGameplayStatics::CreatePlayer(GetWorld(), j);
//					c->Possess(player);
//					ADumpster_FireCharacter* character = Cast<ADumpster_FireCharacter>(player);
//					character->m_Index = j;
//					
//
//				}
//			}
//		}
//
//	}
//}

void ADumpster_FireGameMode::PossessPlayers()
{

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(m_pPlayers.Num()));

	for (int i = 0; i < m_pPlayerControllers.Num(); i++)
	{
		m_pPlayerControllers[i]->Possess(m_pPlayers[i]);
	}
}

void ADumpster_FireGameMode::SpawnInputReceivers()
{

	for (int i = 0; i < m_pPlayerStarts.Num(); i++)
	{
		m_index = i;
		auto playerStart = m_pPlayerStarts[m_index];
		auto PlayerCast = Cast<APlayerStart>(playerStart);
		auto playerTag = PlayerCast->PlayerStartTag;

		for (int j = 0; j < m_pPlayerStarts.Num(); j++)
		{
			if (FString::FromInt(j) == playerTag.ToString())
			{
				m_index = i;
				FVector spawnPoint = playerStart->GetActorLocation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				if (m_index == 0)
				{
					auto receiver = GetWorld()->SpawnActor<APawn>(m_pInputReceiver, spawnPoint, FRotator(0, 0, 0), FActorSpawnParameters{});
					auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					controller->Possess(receiver);
					AInputReceiver* inputReceiver = Cast<AInputReceiver>(receiver);
					inputReceiver->SetId(0);

				}
				else
				{
					auto receiver = GetWorld()->SpawnActor<APawn>(m_pInputReceiver, spawnPoint, FRotator(0, 0, 0), FActorSpawnParameters{});
					if (receiver)
					{
						auto c = UGameplayStatics::CreatePlayer(GetWorld(), m_index);
						if (c)
						{
							c->Possess(receiver);

							AInputReceiver* inputReceiver = Cast<AInputReceiver>(receiver);
							inputReceiver->SetId(m_index);
						}

						else
						{
							auto c2 = UGameplayStatics::GetPlayerController(GetWorld(), m_index);
							c2->Possess(receiver);
							AInputReceiver* inputReceiver = Cast<AInputReceiver>(receiver);
							inputReceiver->SetId(m_index);
						}
					}

				}
			}
		}

	}

	m_HasSpawned = true;
}

