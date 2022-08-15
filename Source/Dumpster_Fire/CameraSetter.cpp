// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSetter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraSetter::ACameraSetter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraSetter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraSetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* pPlayer = UGameplayStatics::GetPlayerController(this, 0);

	if ((pPlayer))
		pPlayer->SetViewTarget(m_pCamera);
}

