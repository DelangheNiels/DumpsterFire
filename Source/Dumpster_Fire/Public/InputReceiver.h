// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputReceiver.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AInputReceiver : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInputReceiver();

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<APawn>> m_Players{};

	void SetId(int id);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		bool GetHasSpawnedPlayer();

private:

	int m_Id{};
	bool m_HasSpawnedPlayer{ false };

	void JoinGame();

};
