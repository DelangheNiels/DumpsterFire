// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkyBox.generated.h"

UCLASS()
class DUMPSTER_FIRE_API ASkyBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyBox();

	UPROPERTY(EditAnywhere)
		UMaterialInstance* m_pMaterial;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_pStaticMesh;

	UPROPERTY(EditAnywhere)
		float m_RotationSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float m_Rotation{ 0 };

};
