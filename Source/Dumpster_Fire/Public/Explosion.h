// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Explosion.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking Collusion")
		class USphereComponent* m_pSphereCollision;


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	URadialForceComponent* m_pRadialForce;

	UNiagaraSystem* m_ParticleSystem;
	UNiagaraComponent* m_ParticleComponent = nullptr;
	UNiagaraComponent* m_ParticleComponent2 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Explosion Setting")
		float m_ImpulseStrength{ 3000.0f };

	UPROPERTY(EditAnywhere, Category = "Explosion Setting")
		float m_Radius{ 350.0f };

	float m_LifeTime{ 0 };
	float m_MaxLifeTime{ 0.2f };

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);





};
