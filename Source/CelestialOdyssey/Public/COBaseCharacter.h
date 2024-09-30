#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "COBaseCharacter.generated.h"

UCLASS()
class CELESTIALODYSSEY_API ACOBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//Constructor
	ACOBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Common functions for all characters
	virtual void MoveRight(float Value);

protected:
	//Movement speed (differs between characters)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MoveSpeed;
};
