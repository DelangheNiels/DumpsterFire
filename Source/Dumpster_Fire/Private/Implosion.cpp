// Fill out your copyright notice in the Description page of Project Settings.


#include "Implosion.h"

#include "Engine/Engine.h"

// Sets default values
AImplosion::AImplosion()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Impulse Created"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Trash force"));

	m_pRadialForce->Radius = m_radius;
	m_pRadialForce->Falloff = RIF_Linear;

	m_pRadialForce->ImpulseStrength = -m_ImpulseStrength;
	m_pRadialForce->bIgnoreOwningActor = false;
	m_pRadialForce->bImpulseVelChange = true;

	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery1);
	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery2);
	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery3);
	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery4);
}



// Called when the game starts or when spawned
void AImplosion::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AImplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_totalTime += DeltaTime;
	m_totalTillNextPulse += DeltaTime;

	if (m_totalTillNextPulse >= 0.1f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Impulse done"));
		m_pRadialForce->FireImpulse();
		m_totalTillNextPulse = 0;
	}

	if (m_totalTime >= 10.0f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Impulse Destroyed"));
		Destroy();

	}

}

