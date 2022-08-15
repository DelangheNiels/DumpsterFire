// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Dumpster_FireCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraActor.h"
#include "NiagaraFunctionLibrary.h"
#include "StartButton.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AStartButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_IsGameStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Incinerator")
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Particles")
		TArray<ANiagaraActor*> m_pPlayerCircles{};

	
private:
	TArray<AActor*> m_pPlayers{};

	void DeactivateRings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
