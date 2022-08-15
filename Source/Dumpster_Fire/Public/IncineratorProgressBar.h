// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Incinerator.h"
#include "IncineratorProgressBar.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AIncineratorProgressBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIncineratorProgressBar();

	UPROPERTY(EditAnywhere, Category = "Progress bar")
		UStaticMeshComponent* m_pMesh;

	UPROPERTY(EditAnywhere, Category = "Progress bar")
		UMaterialInstance* m_pMaterial;

	UPROPERTY(EditAnywhere, Category = "Progress bar")
		AIncinerator* m_pIncinerator;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float m_ScalePercentage;
	float m_MaxScale = 2.9f;

};
