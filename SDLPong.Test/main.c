#include <stdio.h>
#include <string.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <stdbool.h>

#include <crtdbg.h>
#include <SDL3/SDL_stdinc.h>


// cl.exe main.c

/*

cl main.c /Z7 /link user32.lib

/Z7 	debug
/Od 	debug build
/Od		turn off optimazation
/O1  	smaller exe
/O2  	faster, bigger exe (DEFAULT)

*/

enum  CommandType {
	PLAY,
	MOVE,
	JUMP,
	NONE
};


struct Play {
	enum CommandType type;
	char sound[20];
};

struct Move {
	enum CommandType type;
	float x;
	float y;
};

union Command {
	enum CommandType type;
	struct Play play;
	struct Move move;
};

union Command commands[10]={0};

union Command* get_commands1(void)
{

	/*union Command* commands = malloc(sizeof(union Command) * 3);*/

	commands[0].type = PLAY;
	strcpy_s(commands[0].play.sound, 20, "lsdjfa.opus");

	commands[1].type = MOVE;
	commands[1].move.x = 10.0f;
	commands[1].move.y = 22.0f;

	commands[2].type = NONE;

	return commands;
}





//int argc, char *argv[]
int main(void) {


	//union Command* commands = get_commands1();

	get_commands1();


	int i = 0;
	while (commands[i].type != NONE) {
		switch (commands[i].type) {
		case PLAY:
			printf("PLAY %s\n", commands[i].play.sound);
			break;
		case MOVE:
			printf("MOVE %f %f\n", commands[i].move.x, commands[i].move.y);
			break;
		case JUMP:
			printf("JUMP");
			break;
		case NONE:
			break;
		}


		i++;
	}

	//free(commands);


	
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}