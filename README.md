The goal of the game is to get three coins (marked as a C on the map). The player and AI alternate moves, but some terrain types take multiple turns to move through.
The AI uses A* algorithm with a custom heuristic to make its decisions on how best to move. To either intercept the player, or head to the coin to wait for the player.

Once the player and AI are close to each other a Rock/Paper/Scissors style combat system happens.
The combat AI uses an N-gram implementation to recognize patterns and guess the players next move.