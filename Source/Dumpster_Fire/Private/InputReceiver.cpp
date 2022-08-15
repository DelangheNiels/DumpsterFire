// Fill out your copyright notice in the Description page of Project Settings.


#include "InputReceiver.h"
#include "../Dumpster_FireCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInputReceiver::AInputReceiver()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInputReceiver::SetId(int id)
{
	m_Id = id;
}

// Called when the game starts or when spawned
void AInputReceiver::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AInputReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(m_Id));

}

// Called to bind functionality to input
void AInputReceiver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Join", IE_Released, this, &AInputReceiver::JoinGame);

}

bool AInputReceiver::GetHasSpawnedPlayer()
{
	return m_HasSpawnedPlayer;
}

void AInputReceiver::JoinGame()
{
	
		FVector spawnPoint = GetActorLocation();
		//
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		auto* player = GetWorld()->SpawnActor<APawn>(m_Players[m_Id], spawnPoint, FRotator(0, 0, 0), FActorSpawnParameters{});
		auto controller = UGameplayStatics::GetPlayerController(GetWorld(), m_Id);
		controller->UnPossess();
		controller->Possess(player);
		ADumpster_FireCharacter* character = Cast<ADumpster_FireCharacter>(player);
		character->m_Index = m_Id;

		m_HasSpawnedPlayer = true;
		
	
	
}

