// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "Components/SphereComponent.h"
#include "../Dumpster_FireCharacter.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Trash force"));

	m_pRadialForce->Radius = m_Radius;
	m_pRadialForce->Falloff = RIF_Linear;

	m_pRadialForce->ImpulseStrength = m_ImpulseStrength;
	m_pRadialForce->bIgnoreOwningActor = false;
	m_pRadialForce->bImpulseVelChange = true;

	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery1);
	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery2);
	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery3);
	m_pRadialForce->AddObjectTypeToAffect(ObjectTypeQuery4);

	m_pSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Attacking Cullusion"));
	m_pSphereCollision->SetSphereRadius(m_Radius-100);
	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };
	m_pSphereCollision->SetupAttachment(m_pRadialForce);
	m_pSphereCollision->SetCollisionProfileName("Trigger");
	m_pSphereCollision->SetRelativeLocation(FVector(0, 0, 0));

	//Particle
	m_ParticleSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem '/Game/Particles/Explosion/NS_Explosion.NS_Explosion'"));
	


}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	m_pSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::OnOverlapBegin);

	m_pRadialForce->FireImpulse();

	m_ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_ParticleSystem, GetActorLocation(), FRotator(0, 0, 0),FVector(2, 2, 1));
	m_ParticleComponent2 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_ParticleSystem, GetActorLocation(), FRotator(0,180,0), FVector(2,2,1));
	
	
	
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_LifeTime += DeltaTime;

	if (m_LifeTime >= m_MaxLifeTime)
	{
		Destroy();
	}
}

void AExplosion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		auto player = Cast<ADumpster_FireCharacter>(OtherActor);
		player->PlayerDied();
	}
}

