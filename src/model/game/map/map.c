#include <string.h>
#include <stdio.h>
#include <time.h>
#include "./place/event/event.h"
#include "./place/place.h"
#include "map.h"

int MAP_HEIGHT = 11;
int MAP_WIDTH = 4;



position position_init(int x, int y)
{
    position p;
    p.x = x;
    p.y = y;
    return p;
}

void printtamere(char * chaine){
    printf("---------------------------------\n TA MERE %s\n---------------------------------\n", chaine);
    fflush(stdout);
}

position player_position(map* ma){
    return ma->position_player;
}

int* playable_move(map* m){
    switch (m->position_player.y)
    {
    case 0:
        return (int[]){0,1};
        break;
    
    case 1:
        return (int[]){0,1,2};
        break;
    
    case 2:
        return (int[]){1,2,3};
        break;
    
    case 3:
        return (int[]){2,3};
        break;
    
    default:
        return (int[]){-1};
        break;
    }
}

void set_event(map *m){
    int random_boss = (int) (1+(rand()%4));
    int random_sanctuary = (int) (1+(rand()%4));
    printf("\npa %d\n", random_boss);
    printf("\nyo %d\n", random_sanctuary);
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if(m->places[i][j].isWhat >= 1 ){
                if(i < 5 && i == random_boss ){
                    m->places[i][j].isWhat = 3;
                    printf("\nrandom_boss : %d\n", random_boss);
                    random_boss = (int) ((6+(rand()%3)));
                }else if(i > 5 && i == random_boss){
                    m->places[i][j].isWhat = 3;
                    printf("\nrandom_boss : %d\n", random_boss);
                    random_boss = (int) (((i+1)+(rand()%(MAP_HEIGHT-(i+1)))));
                    printf("\nrandom_boss : %d\n", random_boss);

                }
                if(i < 5 && i == random_sanctuary ){
                    if(m->places[i][j].isWhat == 3){
                        m->places[i][j].isWhat = 2;
                    }else{
                        m->places[i][j].isWhat = 2;
                    }
                    printf("\nrandom_sanctuary : %d\n", random_sanctuary);
                    random_sanctuary = (int) ((6+(rand()%3)));
                }else if(i > 5 && i == random_sanctuary){
                    if(m->places[i][j].isWhat == 3){
                        printf("fhdsqmhg");
                        m->places[i][j+random_sanctuary%MAP_WIDTH].isWhat = 2;
                    }else{
                        m->places[i][j].isWhat = 2;
                    }
                    printf("\n random_sanctuary : %d\n", random_sanctuary);
                }
            }   
        }
        
    }
    
}

map* map_init()
{
    map *m = malloc(sizeof(map));
    srand(time(NULL));
    int random = 0;
    m->position_player = position_init(0, 0);
    m->places = calloc(MAP_HEIGHT, sizeof(place*));
    int i,j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        if (i == 0 || i == MAP_HEIGHT - 1){
            m->places[i] = calloc(1, sizeof(place));
        }else{
            m->places[i] = calloc(MAP_WIDTH, sizeof(place));
        }
        random =(int) (rand() % MAP_WIDTH);	// 0, 1, 2, 3
        for (j = 0; j < ((i==0||i==MAP_HEIGHT-1)?1:MAP_WIDTH); j++)
        {
            if(i == 5){
                m->places[i][j] = place_init(2, NULL, NULL);
            }else if (random == j){
                m->places[i][j] = place_init(1, NULL, NULL);
            }else{
                m->places[i][j] = place_init(0, NULL, NULL);
            }
        }
    }
    set_event(m);
    return m;
}

boolean check_map(map * m, int y)
{   
    if (m->position_player.x == 0 && y <= MAP_WIDTH -1  && y >= 0)
    {
        return true;
    }
    if(m->position_player.y <= MAP_WIDTH - 2 && m->position_player.y >= 1){
        if( y-1 == m->position_player.y ||  y+1 == m->position_player.y || y == m->position_player.y ){
            return true;
        } 
    } else if(m->position_player.y == 0){
        if( y-1 == m->position_player.y || y == m->position_player.y ){
            return true;
        }
    } else if( m->position_player.y == MAP_WIDTH - 1){
        if( y+1 == m->position_player.y || y == m->position_player.y ){
            return true;
        }
    }
    
    return false;
}

void move_player(map* m, int y)
{
    if(check_map(m, y)){
        m->position_player = position_init(m->position_player.x+1,y);
        printf("position %d , %d \n", m->position_player.x, m->position_player.y);
    }else{
        printf("Impossible de se déplacer \n");
    }
}

void map_print(map* m)
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < ((i==0||i==MAP_HEIGHT-1)?1:MAP_WIDTH); j++)
        {
            printf("%d", m->places[i][j].isWhat);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
}

//returns the event of the given tile
place map_get(map* map,int x, int y)
{
    return map->places[x][y];
}

void testMap()
{
    map *m = map_init();
    map_print(m);
    move_player(m,2);
    move_player(m,5);
    move_player(m,0);
    move_player(m,1);
    move_player(m,0);
    move_player(m,3);
    move_player(m,0);
    move_player(m,1);
    move_player(m,2);
    move_player(m,3);
    printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
}
