#include <string.h>
#include <stdio.h>
#include <time.h>
// #include "./place/event/event.h"
#include "./place/place.h"
#include "map.h"

int MAP_HEIGHT = 11;
int MAP_WIDTH = 4;

position_player position_init(int x, int y)
{
    position_player p;
    p.x = x;
    p.y = y;
    return p;
}

void printtamere(char *chaine)
{
    printf("---------------------------------\n TA MERE %s\n---------------------------------\n", chaine);
    fflush(stdout);
}

position_player player_position(map *ma)
{
    return ma->position_player;
}

int *playable_move(map *m)
{
    switch (m->position_player.y)
    {
    case 0:
        return (int[]){0, 1};
        break;

    case 1:
        return (int[]){0, 1, 2};
        break;

    case 2:
        return (int[]){1, 2, 3};
        break;

    case 3:
        return (int[]){2, 3};
        break;

    default:
        return (int[]){-1};
        break;
    }
}

boolean* event_place(){
    boolean *array = malloc(sizeof(boolean)*MAP_HEIGHT-2);
    for (int i = 0; i < MAP_HEIGHT-2; i++)
    {
        array[i] = false;
    }
    int random = (rand()%(MAP_HEIGHT-3));
    boolean flag = false;
    array[4] = true;
    int j = 0;
    for (int i = 0; i < 5; i++)
    {
        do
        {
            if(array[random] == false){
                array[random] = true;
                flag = true;
                j ++;
            } 
            random = (rand()%(MAP_HEIGHT-3));
        } while (!flag);
        flag = false;   
    }
    
    return array;
}

void set_event(map *m){
    boolean *where = event_place();
    int sanctuary = 2;
    int boss = 3;
    int random = 0;
    for (int i = 1; i < MAP_HEIGHT-1; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if(m->places[i][j].isWhat == 1){
                if(where[i-1] == true){
                    // printf("random = %d\n", random);
                    if((random%2) && sanctuary != 0){
                        m->places[i][j].isWhat = 2;
                        sanctuary--;
                    }else if (!(random%2) && boss != 0){
                        m->places[i][j].isWhat = 3;
                        boss--;
                    }
                    random ++;
                    // if(!random && sanctuary != 0){
                    //     random = (random%2)+1;
                    // }if(random && boss != 0){
                    //     random = (random%2)+1;
                    // }
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
    m->position_player = position_init(0, 1);
    m->places = calloc(MAP_HEIGHT, sizeof(place *));
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        if (i == 0 || i == MAP_HEIGHT - 1)
        {
            m->places[i] = calloc(1, sizeof(place));
        }
        else
        {
            m->places[i] = calloc(MAP_WIDTH, sizeof(place));
        }
        random =(rand() % MAP_WIDTH);	// 0, 1, 2, 3
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

boolean check_map(map *m, int y)
{
    if (m->position_player.x == 0 && y <= MAP_WIDTH - 1 && y >= 0)
    {
        return true;
    }
    if (m->position_player.y <= MAP_WIDTH - 2 && m->position_player.y >= 1)
    {
        if (y - 1 == m->position_player.y || y + 1 == m->position_player.y || y == m->position_player.y)
        {
            return true;
        }
    }
    else if (m->position_player.y == 0)
    {
        if (y - 1 == m->position_player.y || y == m->position_player.y)
        {
            return true;
        }
    }
    else if (m->position_player.y == MAP_WIDTH - 1)
    {
        if (y + 1 == m->position_player.y || y == m->position_player.y)
        {
            return true;
        }
    }

    return false;
}

void move_player(map *m, int y)
{
    if (check_map(m, y))
    {
        m->position_player = position_init(m->position_player.x + 1, y);
        printf("position %d , %d \n", m->position_player.x, m->position_player.y);
    }
    else
    {
        printf("Impossible de se déplacer \n");
    }
}

void map_print(map *m)
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < ((i == 0 || i == MAP_HEIGHT - 1) ? 1 : MAP_WIDTH); j++)
        {
            printf("%d", m->places[i][j].isWhat);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
}

// returns the event of the given tile
int map_get(map *map, int x, int y)
{
    return map->places[x][y].isWhat;
}

void testMap()
{
    Test_EVENT();
    map *m = map_init();
    map_print(m);
    printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
    move_player(m, 3);
    move_player(m, 5);
    move_player(m, 0);
    move_player(m, 1);
    move_player(m, 0);
    move_player(m, 3);
    move_player(m, 0);
    move_player(m, 1);
    move_player(m, 2);
    move_player(m, 3);
    printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
}
