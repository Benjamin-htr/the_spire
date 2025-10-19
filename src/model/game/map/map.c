#include <string.h>
#include <stdio.h>
#include <time.h>
#include "map.h"

int MAP_HEIGHT = 11;
int MAP_WIDTH = 4;

// init la position du player

position_player position_init(int x, int y)
{
    position_player p;
    p.x = x;
    p.y = y;
    return p;
}

// permet de debugger quand on est en colère

void printtamere(char *chaine)
{
    printf("---------------------------------\n TA MERE %s\n---------------------------------\n", chaine);
    fflush(stdout);
}

// retourne la position actuelle du joueur

position_player player_position(map *ma)
{
    return ma->position_player;
}

// retourne les positions jouable

const int *playable_move(map *m)
{
    switch (m->position_player.y)
    {
    case 0:
        {
            static const int moves_0[] = {0, 1};
            return moves_0;
        }
        break;

    case 1:
        {
            static const int moves_1[] = {0, 1, 2};
            return moves_1;
        }
        break;

    case 2:
        {
            static const int moves_2[] = {1, 2, 3};
            return moves_2;
        }
        break;

    case 3:
        {
            static const int moves_3[] = {2, 3};
            return moves_3;
        }
        break;

    default:
        {
            static const int moves_invalid[] = {-1};
            return moves_invalid;
        }
        break;
    }
}

// affiche l'event actuelle
void go_event(map *m)
{
    switch (map_get(m))
    {
    case 0:
        printf("You are in a combat\n");
        break;
    case 1:
        printf("You are in an event\n");
        break;
    case 2:
        printf("You are in a sanctuaire\n");
        break;
    case 3:
        printf("You are in a miniboss\n");
        break;

    default:
        break;
    }
}

// liée a l'evenement tp permet de tp le joueur

void *teleporter(map *m)
{
    printf("teleporter\n");
    int random = rand() % 6;
    boolean isTP = false;
    const int *move = playable_move(m);
    int lenght = 3;
    if (m->position_player.y == 3 || m->position_player.y == 0)
    {
        lenght = 2;
    }
    printf("lenght = %d\n", lenght);
    if (m->position_player.x != 0 && m->position_player.x != MAP_WIDTH - 1)
    {
        if (random < 3)
        {
            isTP = true;
        }
        move_player(m, move[random % lenght], isTP);
    }
    return NULL;
}

// retour un tableau de boolean d'ou sont les evenements sur la map

boolean *event_place()
{
    // allocate MAP_HEIGHT-2 booleans; ensure correct precedence with parentheses
    boolean *array = malloc(sizeof(boolean) * (MAP_HEIGHT - 2));
    for (int i = 0; i < MAP_HEIGHT - 2; i++)
    {
        array[i] = false;
    }
    int random = (rand() % (MAP_HEIGHT - 3));
    boolean flag = false;
    array[4] = true;
    for (int i = 0; i < 5; i++)
    {
        do
        {
            if (array[random] == false)
            {
                array[random] = true;
                flag = true;
            }
            random = (rand() % (MAP_HEIGHT - 3));
        } while (!flag);
        flag = false;
    }

    return array;
}

// lie le tableau de boolean des evenements à la map

void set_event(map *m)
{
    boolean *where = event_place();
    int sanctuary = 2;
    int boss = 3;
    int random = 0;
    for (int i = 1; i < MAP_HEIGHT - 1; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (m->places[i][j].isWhat == 1)
            {
                if (where[i - 1] == true)
                {
                    if ((random % 2) && sanctuary != 0)
                    {
                        m->places[i][j].isWhat = 2;
                        m->places[i][j].eventData = get_sanctuary();
                        sanctuary--;
                    }
                    else if (!(random % 2) && boss != 0)
                    {
                        m->places[i][j].isWhat = 3;
                        m->places[i][j].eventData = get_mini_boss();
                        m->places[i][j].eventData->data = getRandomMiniBoss();
                        boss--;
                    }
                    random++;
                }
            }
        }
    }
}

// init de la map

map *map_init()
{
    map *m = malloc(sizeof(map));

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
        random = (rand() % MAP_WIDTH); // 0, 1, 2, 3
        for (j = 0; j < ((i == 0 || i == MAP_HEIGHT - 1) ? 1 : MAP_WIDTH); j++)
        {
            if (i == 5)
            {
                m->places[i][j] = place_init(2, get_sanctuary(), NULL);
            }
            else if (random == j)
            {
                m->places[i][j] = place_init(1, get_random_event(), NULL);
            }
            else
            {
                m->places[i][j] = place_init(0, NULL, i > 5 ? getRandomEnemyPhase2() : getRandomEnemyPhase1());
            }
        }
        if (i == MAP_HEIGHT - 1)
        {
            m->places[i][0] = place_init(0, NULL, importBOSSFromId(GARDIAN_PLUME));
        }
    }
    set_event(m);
    return m;
}

// check si le joueur peut bouger a une position

boolean check_map(map *m, int y)
{
    // Special case: from the last normal floor (index MAP_HEIGHT-2), always allow moving to boss column 0
    if (m->position_player.x == MAP_HEIGHT - 2 && y == 0)
    {
        return true;
    }
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

// move le player si checkmap retourne true

void move_player(map *m, int y, boolean isTP)
{
    if (check_map(m, y))
    {
        if (isTP)
        {
            m->position_player = position_init(m->position_player.x - 1, y);
        }
        else
        {
            m->position_player = position_init(m->position_player.x + 1, y);
        }
        printf("position %d , %d \n", m->position_player.x, m->position_player.y);
        // go_event(m);
    }
    else
    {
        printf("Impossible de se déplacer \n");
    }
}

// print la map

void map_print(map *m)
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < ((i == 0 || i == MAP_HEIGHT - 1) ? 1 : MAP_WIDTH); j++)
        {
            printf("%d", m->places[i][j].isWhat);
        }
        printf("\n");
    }
}

// retourne le type d'evenement du joueur

int map_get(map *map)
{
    printf("isWhat value at position (%d, %d): %d\n", map->position_player.x, map->position_player.y, map->places[map->position_player.x][map->position_player.y].isWhat);
    return map->places[map->position_player.x][map->position_player.y].isWhat;
}

// test de la map

void testMap()
{
    Test_EVENT();
    // map *m = map_init();
    //  map_print(m);
    //  displayEntity(m->places[MAP_HEIGHT - 1][0].enemyData);
    //  printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
    //  // move_player(m, 3, false);
    //  // move_player(m, 5, false);
    //  move_player(m, 0, false);
    //  printf("\n\n");
    //  // move_player(m, 1, false);
    //  printf("position %d , %d \n", m->position_player.x, m->position_player.y);
    //  printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
    //  printf("\n\n");

    // teleporter(m);
    // printf("\n\n");

    // printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
    // printf("position %d , %d \n", m->position_player.x, m->position_player.y);
    // printf("\n\n");

    // move_player(m, 0, false);
    // move_player(m, 3, false);
    // move_player(m, 0, false);
    // move_player(m, 1, false);
    // move_player(m, 2, false);
    // move_player(m, 3, false);
    // printf("playable move : %d, %d\n", playable_move(m)[0], playable_move(m)[1]);
}
