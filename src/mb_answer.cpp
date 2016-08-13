#include "mb_answer.h"

#include <iostream>
#include <fstream>

using namespace std;

MBAnswer::MBAnswer(){
	answer = fopen("res/answer.dat", "rb");

	if(not answer){
		printf("Não foi possível abrir o arquivo gabarito.dat\n");
		exit(1);
	}

	fread(&n_levels, sizeof(int), 1, answer);

	int id_level=0;
	while(fread(&m_frames[++id_level], sizeof(int), 1, answer)>0){
		fread(m_level_answer[id_level][0], sizeof(int), m_frames[id_level], answer);
		fread(m_level_answer[id_level][1], sizeof(int), m_frames[id_level], answer);
		fread(m_level_answer[id_level][2], sizeof(int), m_frames[id_level], answer);
	}

	fclose(answer);
}

int MBAnswer::n_frames(int level){
	return m_frames[level]; 
}

int MBAnswer::in_trail(int level, int frame){
	return m_level_answer[level][0][frame-1];
}

int MBAnswer::main_trail(int level, int frame){
	return m_level_answer[level][1][frame-1];
}

int MBAnswer::out_trail(int level, int frame){
	return m_level_answer[level][2][frame-1];
}

MBAnswer::~MBAnswer(){
	fclose(answer);
	free(m_level_answer);
}
