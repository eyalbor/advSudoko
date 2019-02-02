#ifndef PARCER_H_
#define PARCER_H_

ADTErr parcer_doCommand(Game* _game, char* _command);

/**
 *
 */
ADTErr parse_file (FILE* fp, Game* _game);

#endif
