#ifndef PARCER_H_
#define PARCER_H_

ADTErr parcer_doCommand(Game* _game, char* _command);

/**
 ** parse_file - The function parses a file and creates a board according to it and to the game's mode.
* @Input
* 	fp - the file we are reading from
* _game - pointer to the game
* @return
* 	ERR_Ok
* 	or EXIT - error occurred and we exit the game.
 */
ADTErr parse_file (FILE* fp, Game* _game);

#endif
