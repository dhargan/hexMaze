#include "maze.h"
#include <iostream>
#include <stack>
#include <stdlib.h>     
#include <time.h>       

/* ---------- CELL --------- */

Cell::Cell(int id){
	for(int i=0; i<6; i++){
		this->neighbors[i] = NULL;
		this->walls[i] = true;
	}
	this->id = id;
	this->NVN = 0;
	this->visited = false;
}

bool Cell::isVisited(){
	return this->visited;
}

void Cell::incNVN(){
	this->NVN++;
}

void Cell::decNVN(){
	this->NVN--;
}

bool Cell::isAllNeighborsVisited(){
	return this->NVN == 0;
}

bool Cell::isValidNB(int cur, int nb, int w){
	if(cur%w == (w-1) && (nb == 1 || nb == 2)){
		return false;
	}
	
	if(cur%w == 0 && (nb == 4 || nb == 5)){
		return false;
	}
	
	return true;
}

void Cell::setNeighbor(int i, int nb, Maze* maze){ // i : direction, nb : id of the neighbor cell
	if(nb<maze->getSize() && nb>=0 && isValidNB(this->getID(),i,maze->getWidth())){
		neighbors[i] = maze->getCell(nb);
		incNVN();
	}else{
		neighbors[i] = NULL;
	}
}

Cell* Cell::getNeighbor(int i){
	return this->neighbors[i];
}

bool Cell::isWallExist(int i){
	return this->walls[i];
}

void Cell::visit(){
	this->visited=true;
	for(int i=0; i<6; i++){
		if(this->neighbors[i] != NULL){
			this->neighbors[i]->decNVN();
		}
	}
}

int Cell::getID(){
	return this->id;
}

int Cell::getNVN(){
	return this->NVN;
}

void Cell::removeWall(int dir){
	this->walls[dir] = false;
	if(dir == 0){
		this->neighbors[0]->walls[3] = false;
	}
	if(dir == 1){
		this->neighbors[1]->walls[4] = false;
	}
	if(dir == 2){
		this->neighbors[2]->walls[5] = false;
	}
	if(dir == 3){
		this->neighbors[3]->walls[0] = false;
	}
	if(dir == 4){
		this->neighbors[4]->walls[1] = false;
	}
	if(dir == 5){
		this->neighbors[5]->walls[2] = false;
	}					
}



/* ----------- MAZE ------------ */



Maze::Maze(int h, int w){
	int c;
	this->height = h;
	this->width = w;
	this->size = h*w;
	this->cells = new Cell*[size];
	for(int i=0; i<size; i++){		
		cells[i] = new Cell(i);		
	}
	
	for(int i=0; i<size; i++){ // Setting the neighbors of the maze graph
		c = i%w;
		if(c%2 == 0){ //x
			cells[i]->setNeighbor(2,i+(w+1),this);
			cells[i]->setNeighbor(4,i+(w-1),this);
			cells[i]->setNeighbor(1,i+1,this);
			cells[i]->setNeighbor(5,i-1,this);
		}else{ //y
			cells[i]->setNeighbor(2,i+1,this);
			cells[i]->setNeighbor(4,i-1,this);
			cells[i]->setNeighbor(1,i-(w-1),this);
			cells[i]->setNeighbor(5,i-(w+1),this);						
		}
		
		cells[i]->setNeighbor(0,i-w,this); // Top (0)
		cells[i]->setNeighbor(3,i+w,this); // Bottom(3)
	}
	
	this->generateMaze();
	//this->printCells();
}

int Maze::getSize(){
	return size;
}

Cell* Maze::getCell(int i){
	return cells[i];
}

void Maze::removeWall(int from, int dir){
	this->cells[from]->removeWall(dir);	
}

void Maze::generateMaze(){
	std::stack<Cell *> mSt;
	mSt.push(this->cells[0]);
	this->cells[0]->visit();
	srand (time(NULL));
	int choosenCell;
	int debugID;
	while(!mSt.empty()){
		choosenCell = rand()%6;
		if(mSt.top()->isAllNeighborsVisited()){
			std::cout<<"back from "<<mSt.top()->getID()<<std::endl;
			mSt.pop();
		}else{
			while(true){
				if(mSt.top()->getNeighbor(choosenCell) != NULL){
					debugID = mSt.top()->getNeighbor(choosenCell)->getID();
					if(!mSt.top()->getNeighbor(choosenCell)->isVisited()){
						std::cout<<"from "<<mSt.top()->getID()<<" to "<<mSt.top()->getNeighbor(choosenCell)->getID()<<std::endl;
						this->removeWall(mSt.top()->getID(), choosenCell);
						mSt.push(mSt.top()->getNeighbor(choosenCell));						
						mSt.top()->visit();
						break;
					}
				}
				choosenCell = rand()%6;
			}
		}
	}
}



void Maze::draw(){
	std::cout<<"Generating "<<this->height<<" x "<<this->width<<" maze..."<<std::endl;
	std::cout<<std::endl;

	for(int j=0; j<this->width-1; j+=2){
		std::cout<<"   _";	
	}
	
	std::cout<<std::endl;
	
	for(int i = 0; i<this->height+1; i++ ){
		if(i == 0){
			std::cout<<" ";
		}else{
			std::cout<<"\\";
		}	
		
		for(int j=i*this->width; j<(i+1)*this->width-1; j+=2){
			if(this->width%2 !=0){ //odd
				//std::cout<<"_/ \\";
				if(j < this->size){
					if(cells[j]->isWallExist(0)){
						std::cout<<"_";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"_";
				}
				if(j+1 < this->size){
					if(cells[j+1]->isWallExist(5)){
						std::cout<<"/";
					}else{
						std::cout<<" ";
					}
					std::cout<<" ";
					if(cells[j+1]->isWallExist(1)){
						std::cout<<"\\";
					}else{
						std::cout<<" ";
					}					
				}else{
					std::cout<<"/ \\";
				}
			}else{
				if(i == this->height){ 
					if(j+2 >=(i+1)*this->width-1){
						std::cout<<"_/";
					}else{
						std::cout<<"_/ \\";
					}
				}else{
					//std::cout<<"_/ \\";

					if(j < this->size){
						if(cells[j]->isWallExist(0)){
							std::cout<<"_";
						}else{
							std::cout<<" ";
						}
					}else{
						std::cout<<" ";
					}
					if(j+1 < this->size){
						if(cells[j+1]->isWallExist(5)){
							std::cout<<"/";
						}else{
							std::cout<<" ";
						}
						std::cout<<" ";
						if(cells[j+1]->isWallExist(1)){
							std::cout<<"\\";
						}else{
							std::cout<<" ";
						}					
					}else{
						std::cout<<"_/ \\";
					}
					
				}
			}					
		}
		if(this->width%2!=0){ //PROBLEM //odd
			if(i != 0){
				//std::cout<<"_/";
				if(((i+1)*width)-1 < size){
					if(cells[((i+1)*width)-1]->isWallExist(0)){
						std::cout<<"_";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"_";
				}
				std::cout<<"/";								
			}else{
				//std::cout<<"_";
				if(((i+1)*width)-1 < size){
					if(cells[((i+1)*width)-1]->isWallExist(0)){
						std::cout<<"_";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"_";
				}				
			}
		}
		
		std::cout<<std::endl;
		
		if(i != this->height){ 
			for(int j=(i*this->width); j<((i+1)*this->width-1); j+=2){
				//std::cout<<"/ \\_";
				if(j < this->size){
					if(cells[j]->isWallExist(5)){
						std::cout<<"/";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"/";
				}
				
				std::cout<<" ";
				
				if(j < this->size){
					if(cells[j]->isWallExist(1)){
						std::cout<<"\\";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"\\";
				}
				
				if(j+1 < this->size){
					if(cells[j+1]->isWallExist(3)){
						std::cout<<"_";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"_";
				}
			}
			if(this->width%2!=0){ //odd
				//std::cout<<"/ \\";
				if((i+1)*this->width < this->size){
					if(cells[(i+1)*this->width-1]->isWallExist(5)){
						std::cout<<"/";
					}else{
						std::cout<<" ";
					}
				}else{
					std::cout<<"/";
				}
				std::cout<<" \\";
			}else{
				std::cout<<"/";
			}
		}
	
		std::cout<<std::endl;
	}
	
	std::cout<<std::endl;
	std::cout<<std::endl;
}	

void Maze::printCells(){
	
	for(int i=0; i<this->size; i++){
		std::cout<<"#"<<this->cells[i]->getID()<<" neighbors: ";
		for(int j=0; j<6; j++){
			if(this->cells[i]->getNeighbor(j) != NULL){
				std::cout<<cells[i]->getNeighbor(j)->getID() <<" ";
			}else{
				std::cout<<"NULL"<<" ";
			}
		}
		std::cout<<std::endl;		
	}
}

int Maze::getWidth(){
	return this->width;
}

