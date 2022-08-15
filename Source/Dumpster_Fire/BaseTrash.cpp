// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTrash.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Dumpster_FireCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ABaseTrash::ABaseTrash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };
	//rules.LocationRule = EAttachmentRule{ EAttachmentRule::SnapToTarget };
	//rules.RotationRule = EAttachmentRule{ EAttachmentRule::SnapToTarget };
	//rules.ScaleRule = EAttachmentRule{ EAttachmentRule::KeepWorld };

	m_pTrashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trash Mesh"));
	m_pTrashMesh->SetSimulatePhysics(true);
	m_pTrashMesh->AttachToComponent(RootComponent,rules);
	
	//m_pTrashMesh->SetMassOverrideInKg(NAME_None, 15000);
	/*GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("%lld"), m_pTrashMesh->GetMass()));
	m_pTrashMesh->GetBodyInstance()->bOverrideMass = true;
	m_pTrashMesh->SetMassOverrideInKg(NAME_None, 500, true);
	m_pTrashMesh->SetMassScale(NAME_None, 500);
	m_pTrashMesh->SetMassOverrideInKg(NAME_None, 500, true);
	m_pTrashMesh->GetBodyInstance()->UpdateMassProperties();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("%lld"), m_pTrashMesh->GetMass()));*/
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (CubeVisualAsset.Succeeded())
	{
		m_pTrashMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pTrashMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}


	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pCollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	m_pCollisionBox->SetCollisionProfileName("Trigger");
	m_pCollisionBox->SetBoxExtent(FVector(m_meshSize));

	m_pCollisionBox->AttachToComponent(m_pTrashMesh, rules);
	/*m_pCollisionBox->SetVisibility(true);
	m_pCollisionBox->SetHiddenInGame(false);
	m_pCollisionBox->SetVisibleFlag(true);*/

}

// Called when the game starts or when spawned
void ABaseTrash::BeginPlay()
{
	Super::BeginPlay();
	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseTrash::OnOverlapBegin);
	
}

// Called every frame
void ABaseTrash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

int ABaseTrash::GetPoints() const
{
	return m_Points;
}

void ABaseTrash::SetIsThrown(bool isThrown) 
{
	m_isThrown = isThrown;
}

void ABaseTrash::SetPlayer(AActor* pPlayer)
{
	m_pPlayer = pPlayer;
}

void ABaseTrash::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		if(m_isThrown)
		{
			auto character = Cast<ADumpster_FireCharacter>(OtherActor);

			if (OtherActor != m_pPlayer)
			{
				//character->SetStunned(true);
				//GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Stunned"));

				auto playerLocation = character->GetCapsuleComponent()->GetRelativeLocation();
				auto blockLocation = GetActorLocation();
				auto PushDirection = playerLocation - blockLocation;

				character->GetCapsuleComponent()->SetSimulatePhysics(true);
				character->GetCapsuleComponent()->AddImpulse(PushDirection * 500);

				character->StartStunParticle();
				m_pPlayer = nullptr;
			}
			m_pPlayer = nullptr;
			m_isThrown = false;
		}
	}
	else
	{
		if (m_isThrown)
		{
			m_pPlayer = nullptr;
			m_isThrown = false;
		}

		//GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("other"));
			
	}


	

	

}

