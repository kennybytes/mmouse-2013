/*
 *
 *
 * FloodFill.c
 *
 */
#include "FloodFill.h"

void Maze_putWall(Cell maze[][16], int y, int x, int dir) {
    switch(dir) {
        case NORTH:
            mouseMaze[y][x].north = 1;
            if(y >= 0) mouseMaze[y+1][x].south = 1;
            break;
        case EAST:
            mouseMaze[y][x].east = 1;
            if(x <= 15) mouseMaze[y][x-1].west = 1;
            break;
        case SOUTH:
            mouseMaze[y][x].south = 1;
            if(y <= 15) mouseMaze[y-1][x].north = 1;
            break;
        case WEST:
            mouseMaze[y][x].west = 1;
            if(x >=15) mouseMaze[y][x+1].east = 1;
            break;
        default:
            break;
    }
}

// Init the maze
void FloodFill_initMaze(void){
    int i = 0;
    int j = 0;
    int level = 0;
    Cell* tempCell;

    StackInit(&currentLevel);
    StackInit(&nextLevel);

    // init the maze values
    for(i = 15; i >= 0 ; i--) {
        for(j = 0; j < 16; j++) {
            mouseMaze[i][j].floodValue = 255;
            mouseMaze[i][j].x = j;
            mouseMaze[i][j].y = i;
        }
    }

    // Map the initial walls
    for( i = 0; i < 16 ; i++)
        mouseMaze[i][0].west = TRUE;
    for( i = 0; i < 16 ; i++)
        mouseMaze[i][15].east = TRUE;
    for( i = 0; i < 16 ; i++)
        mouseMaze[0][i].south = TRUE;
    for( i = 0; i < 16 ; i++)
        mouseMaze[15][i].north = TRUE;

    //Mouse_printMaze();
    // initialize the maze with the correct flood values
    //printf("%d\n", &mouseMaze[7][7]);

    tempCell = &mouseMaze[7][7];
    StackPush(&currentLevel, &mouseMaze[7][7]);

    while(1){
        while(!StackIsEmpty(&currentLevel)) {
            tempCell=StackPop(&currentLevel);
            //printf("Popping %d,%d \n", tempCell->x, tempCell->y);
            if(tempCell->floodValue == 255) {
                tempCell->floodValue = level;
                FloodFill_pushNeighbors(tempCell);
            }
        }
        if(!StackIsEmpty(&nextLevel)) {
            level++;
            //printf("Level %d\n", level);
            StackCopy(&nextLevel, &currentLevel);   // Copy nextLevel into Current lev
            //StackDestroy(&nextLevel);
            StackInit(&nextLevel);
        }
        else {
            break;  // if empty, done
        }
    }
}


void FloodFill_floodMaze(void){
    int i = 0;
    int j = 0;
    int level = 0;
    Cell* tempCell;

    // init the maze values
    for(i = 15; i >= 0 ; i--) {
        for(j = 0; j < 16; j++) {
            mouseMaze[i][j].floodValue = 255;
            mouseMaze[i][j].x = j;
            mouseMaze[i][j].y = i;
        }
    }

    tempCell = &mouseMaze[7][7];
    StackPush(&currentLevel, &mouseMaze[7][7]);

    while(1){
        while(!StackIsEmpty(&currentLevel)) {
            tempCell=StackPop(&currentLevel);
            //printf("Popping %d,%d \n", tempCell->x, tempCell->y);
            if(tempCell->floodValue == 255) {
                tempCell->floodValue = level;
                FloodFill_pushNeighbors(tempCell);
                //FloodFill_printMaze();
                //printf("\n\n");
            }
        }
        if(!StackIsEmpty(&nextLevel)) {
            level++;
            //printf("Level %d\n", level);
            StackCopy(&nextLevel, &currentLevel);   // Copy nextLevel into Current lev
            //StackDestroy(&nextLevel);
            StackInit(&nextLevel);
        }
        else {
            break;  // if empty, done
        }
    }

}

void FloodFill_printMaze(void)
{
    int o = 0;
    int p = 0;
    for( o = 15 ; o >= 0 ; o--){
        for( p = 0; p < 16 ; p++) {
            printf("%.2d ", mouseMaze[o][p].floodValue);
            printf(" ");
        }
        printf("\n\n");
    }
}

void FloodFill_pushNeighbors(Cell *c) {
    if(!FloodFill_checkWall(c, NORTH)) {
        //printf("pushing north %d,%d\n", c->x, c->y+1);
        StackPush(&nextLevel, &mouseMaze[c->y+1][c->x]);
    }
    if(!FloodFill_checkWall(c, EAST)) {
        //printf("pushing east %d,%d\n", c->x+1, c->y);
        StackPush(&nextLevel, &mouseMaze[c->y][c->x+1]);
    }
    if(!FloodFill_checkWall(c, SOUTH))
    {
        //printf("pushing south %d,%d\n", c->x, c->y-1);
        StackPush(&nextLevel, &mouseMaze[c->y-1][c->x]);
    }
    if(!FloodFill_checkWall(c, WEST))
    {
        //printf("pushing west %d,%d\n", c->x-1, c->y);
        StackPush(&nextLevel, &mouseMaze[c->y][c->x-1]);
    }
}

int FloodFill_checkWall(Cell *c, int d) {
    switch(d)
    { case NORTH:
            if(c->north) return TRUE;
            else return FALSE;
            break;
        case WEST:
            if(c->west) return TRUE;
            else return FALSE;
            break;
        case SOUTH:
            if(c->south) return TRUE;
            else return FALSE;
            break;
        case EAST:
            if(c->east) return TRUE;
            else return FALSE;
            break;
        default:
            return FALSE;
    }
}

int Maze_smallestNeighborDir(Cell *c){
    int smallest = NORTH;
    int floodValue;
    int smallestFloodValue = 100;

    if((c->east)==0){
        floodValue = mouseMaze[c->y][c->x+1].floodValue;
        //printf("east is empty with floodValue %d\n",floodValue );
        if(floodValue < smallestFloodValue) {
            //printf("making east smallest\n");
            smallest = EAST;
        }

    }

    if((c->south)==0){
        floodValue = mouseMaze[c->y-1][c->x].floodValue;
        //printf("south is empty with floodValue %d\n",floodValue );
        if(floodValue < smallestFloodValue)
            smallest = SOUTH;

    }

    if((c->west)==0){
        floodValue = mouseMaze[c->y][c->x-1].floodValue;
        //printf("west is empty with floodValue %d\n",floodValue );
        if(floodValue < smallestFloodValue)
            smallest = WEST;
    }

    if((c->north)==0){
        floodValue = mouseMaze[c->y+1][c->x].floodValue;
        //printf("north is empty with floodValue %d\n", floodValue);
        if(floodValue < smallestFloodValue)
            smallest = NORTH;
    }
    //printf("returning %d\n\n\n\n", smallest);
    return smallest;
}