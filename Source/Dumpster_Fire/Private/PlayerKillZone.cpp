// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerKillZone.h"
#include "../Dumpster_FireCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerKillZone::APlayerKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pCollisionBox->SetCollisionProfileName("Trigger");
	m_pCollisionBox->SetupAttachment(RootComponent);
	m_pCollisionBox->SetBoxExtent(FVector(10000, 10000, 120));
}

// Called when the game starts or when spawned
void APlayerKillZone::BeginPlay()
{
	Super::BeginPlay();
	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerKillZone::OnOverlapBegin);
}

// Called every frame
void APlayerKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerKillZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		auto character = Cast<ADumpster_FireCharacter>(OtherActor);
		character->PlayerDied();
	}
}

