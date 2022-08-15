// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Implosion.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AImplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImplosion();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	URadialForceComponent* m_pRadialForce;

	
	float m_totalTime;

	
	float m_totalTillNextPulse;

	UPROPERTY(EditAnywhere, Category = "Implosion Setting")
	float m_impulseTimer{3.0f};

	UPROPERTY(EditAnywhere, Category = "Implosion Setting")
		float m_ImpulseStrength{ 500.0f };

	UPROPERTY(EditAnywhere, Category = "Implosion Setting")
		float m_radius{ 750.0f };


};
