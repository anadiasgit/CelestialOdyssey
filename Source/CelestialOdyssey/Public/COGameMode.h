#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "COGameMode.generated.h"

/**
 * This class is responsible for setting the default pawn, player controller, and handling any game-wide logic such as game flow, player
 * spawning, and more.
 */
UCLASS()
class CELESTIALODYSSEY_API ACOGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//Constructor
	ACOGameMode();

protected:
	//Called when the game starts
	virtual void StartPlay() override;
};
