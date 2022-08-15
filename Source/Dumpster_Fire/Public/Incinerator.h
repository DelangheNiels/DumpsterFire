// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTimerManager.h"
#include "TrashSpawner.h"
#include "NiagaraComponent.h"
#include "NiagaraActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Incinerator.generated.h"


class AFireChanger;

UCLASS()
class DUMPSTER_FIRE_API AIncinerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIncinerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Incinerator")
		class UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		int m_TeamId{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		int m_PointsToShow{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
		bool m_ShowPoints{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
	float m_MaxInactiveTime{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Incinerator")
	AGameTimerManager* m_pGameManager;

	UPROPERTY(EditAnywhere)
		ANiagaraActor* m_pSmokeParticle;

	UPROPERTY(EditAnywhere)
		ANiagaraActor* m_pBlackHoleParticle;

	UPROPERTY(EditAnywhere, Category = "Incinerator")
	float m_MaxSmokeTime = 2.1f;

	UFUNCTION(BlueprintCallable, Category = "Incinerator")
	int GetTotalPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Incinerator")
		float GetMultiplier() const;

	UFUNCTION(BlueprintCallable, Category = "Incinerator")
		bool GetIsActive() const;

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<UCameraShakeBase> m_CameraShake{ };

	UPROPERTY(EditAnywhere)
		AFireChanger* m_pFireChanger;

	UPROPERTY(EditAnywhere, Category = "Black Hole")
		USoundBase* m_pBlackHoleSound = nullptr;

	void SpawnRisingFire();

	int GetPointsToWin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int m_TotalScore{ 0 };

	bool m_isBlackHoleActive= false;
	bool m_PlaySmoke = false;
	float m_SmokeTime = 0.0f;
	float m_BlackHoleTime = 0.0f;
	float m_MaxBlackTime = 10.0f;


	const std::vector<float> m_scoreMultipliers = { 1.0f, 1.5f,  2.0f };
	int m_index = 0;

	bool m_IsActive{ true };
	float m_ElapsedInactiveTime{ 0.0f };
	
	ATrashSpawner* m_pSpawner;

	int m_PlayerHitCount{ 0 };
	float m_ClearTimer{0};

	bool m_AddPoints{ false };
	int m_PointsToAdd{ 0 };
	float m_AddPointsTimer{ 0.0f };
	float m_AddPointsTime{ 0.05f };

	float m_ShowPointsTimer{ 0.0f };
	float m_ShowPointsTime{ 0.5f};

	bool m_IsRisingFireSpawned{ false };
	float m_SpawnRisingTimer{ 0 };
	float m_RisingFlameTime{ 1.0f };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
