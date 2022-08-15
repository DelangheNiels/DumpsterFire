// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "Incinerator.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraActor.h"
#include "NiagaraFunctionLibrary.h"
#include "FireChanger.generated.h"

UENUM(BlueprintType)
enum class FireType : uint8 {
	Normal = 0,
	Purple = 1,
	Blue = 2
};

UCLASS()
class DUMPSTER_FIRE_API AFireChanger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireChanger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		AIncinerator* m_pIncinerator {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		TArray<ANiagaraActor*> m_pFires{};

	UPROPERTY(EditAnywhere, Category = "Incinerator")
		TArray<ANiagaraActor*> m_pRisingFires{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		TArray<ANiagaraActor*> m_pBlueFires{};

	UPROPERTY(EditAnywhere, Category = "Incinerator")
		TArray<ANiagaraActor*> m_pRisingBlueFires{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		TArray<ANiagaraActor*> m_pPurpleFlames{};

	UPROPERTY(EditAnywhere, Category = "Incinerator")
		TArray<ANiagaraActor*> m_pRisingPurpleFires{};

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* m_pFireFeedBack;

	void SpawnRisingFlames();
	void DespawnRisingFlames();


private:
	UNiagaraSystem* m_pRedFireParticleSystem;
	UNiagaraSystem* m_pBlueFireParticleSystem;

	FireType m_ActiveFireType = FireType::Normal;

	void ActivateRisingFlames(TArray<ANiagaraActor*> fires);
	void DeactivateRisingFlames(TArray<ANiagaraActor*> fires);

	bool m_PlayPurpleRising{ true };
	bool m_PlayBlueRising{ true };





};
