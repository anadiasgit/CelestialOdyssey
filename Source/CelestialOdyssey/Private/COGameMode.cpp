#include "COGameMode.h"
#include "COPlayerCharacter.h"
#include "COPlayerController.h"
#include "COPlayerState.h"

/**
 *  Constructor
 */
ACOGameMode::ACOGameMode()
{
	//Set default player state
	PlayerStateClass = ACOPlayerState::StaticClass();
}

/**
 *  This function is used to initialize any game-specific rules or logic when the game begins.
 *  Override this function to add custom game-start logic for Celestial Odyssey.
 */
void ACOGameMode::StartPlay()
{
	Super::StartPlay();
}
