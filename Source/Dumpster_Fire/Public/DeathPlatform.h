// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathPlatform.generated.h"

UCLASS()
class DUMPSTER_FIRE_API ADeathPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathPlatform();

	UStaticMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
		UStaticMeshComponent* m_pMesh;
		FVector pos = { -150,50,2300 };

};
