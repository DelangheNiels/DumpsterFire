// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Incinerator.h"
#include "Dumpster_Fire/BaseTrash.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BlackHoleBlock.generated.h"



/**
 * 
 */
UCLASS()
class DUMPSTER_FIRE_API ABlackHoleBlock : public ABaseTrash
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlackHoleBlock();

	UPROPERTY(EditAnywhere, Category = "Black Hole")
		USoundBase* m_pBlackHoleSound = nullptr;

	void PullBlocksTowardsIncinerator(AIncinerator* incinerator);

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	bool m_canPulltowardIncinerator{};

	AIncinerator* m_pIncinerator;

	ANiagaraActor* m_pParticle{};


	UPROPERTY(EditAnywhere)
		float m_powerUpTimer{10.f};
		float m_elapsedSec{0.f};
};
