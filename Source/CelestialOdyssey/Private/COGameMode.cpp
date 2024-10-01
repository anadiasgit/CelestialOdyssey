#include "COGameMode.h"
#include "COPlayerCharacter.h"
#include "COPlayerController.h"

/**
 *  Constructor
 */
ACOGameMode::ACOGameMode()
{
}

/**
 *  This function is used to initialize any game-specific rules or logic when the game begins.
 *  Override this function to add custom game-start logic for Celestial Odyssey.
 */
void ACOGameMode::StartPlay()
{
	Super::StartPlay();
}
