// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dumpster_Fire/BaseTrash.h"
#include "Components/AudioComponent.h"
#include "BombBlock.generated.h"

UCLASS()
class DUMPSTER_FIRE_API ABombBlock : public ABaseTrash
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABombBlock();

	UPROPERTY(EditAnywhere, Category = "Implosion Setting")
		float m_ExplosionTimer{ 10.0f };

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* m_pExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* m_pCountdownSound;

	UPROPERTY(EditAnywhere, Category = "Controller Rumble")
		UForceFeedbackEffect* m_pForceFeedback {};

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<UCameraShakeBase> m_CameraShake{ };

	void Explode();


	void DoControllerRumble();

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	float m_ElapsedTime{ 0.0f };
	UAudioComponent* m_pAudioComponent;

};
