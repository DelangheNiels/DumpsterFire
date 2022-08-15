// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_pMesh;

	void SetMoveLeftToRight(bool moveLtR);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	float m_MovementSpeed;
	float m_MinMovementSpeed{ 200.0f };
	float m_MaxMovementSpeed{ 450.0f };

	float m_MinSize{ 0.5f };
	float m_MaxSize{ 1.5f };

	float m_MinUpMovement{ -100.0f };
	float m_MaxUpMovement{ 100.0f };
	float m_UpMovement{};

	int m_MinRotationSpeed{ 30 };
	int m_MaxRotationSpeed{ 80 };
	int m_RandomRotation{};

	int m_RandomAxis{ 0 };

	float m_MaxAliveTime{ 30.0f };

	float m_TimeAlive{ 0.0f };

	bool m_MoveLeftToRight = true;


};
