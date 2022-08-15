// Fill out your copyright notice in the Description page of Project Settings.


#include "StartButton.h"
#include "Components/BoxComponent.h"

// Sets default values
AStartButton::AStartButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_IsGameStarted = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(120, 120, 120));
	CollisionBox->SetCollisionProfileName("Trigger");
	CollisionBox->SetupAttachment(RootComponent);

}

void AStartButton::DeactivateRings()
{
	for (int i = 0; i < m_pPlayerCircles.Num(); i++)
	{
		m_pPlayerCircles[i]->GetNiagaraComponent()->DeactivateImmediate();
	}
}

// Called when the game starts or when spawned
void AStartButton::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStartButton::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AStartButton::OnOverlapEnd);
	
}

// Called every frame
void AStartButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AStartButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		if (!m_pPlayers.Contains(OtherActor))
		{
			m_pPlayers.AddUnique(OtherActor);
			
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(m_pPlayers.Num()));

			if (m_pPlayers.Num() == 4)
			{
				m_IsGameStarted = true;
				DeactivateRings();

			}

			if (!m_IsGameStarted)
			{
				auto player = Cast<ADumpster_FireCharacter>(OtherActor);
				m_pPlayerCircles[player->GetIndex()]->GetNiagaraComponent()->DeactivateImmediate();
			}
			
		}
		
	}
}

void AStartButton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(m_pPlayers.Num()));
		m_pPlayers.Remove(OtherActor);

		if (!m_IsGameStarted)
		{
			auto player = Cast<ADumpster_FireCharacter>(OtherActor);
			m_pPlayerCircles[player->GetIndex()]->GetNiagaraComponent()->Activate();
		}
		
	}
}

