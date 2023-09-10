#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) // Función 2.
{
  long numHash = hash(key, map->capacity);

  while (map->buckets[numHash] != NULL && map->buckets[numHash]->key != NULL)
    {
     numHash = (numHash + 1) % map->capacity; 
    }

  map->buckets[numHash] = createPair(key, value);
  map->size++;
  map->current = numHash;
  
}

void enlarge(HashMap * map)
{
  enlarge_called = 1; //no borrar (testing purposes)

  Pair ** auxBuckets = map->buckets;
  map->capacity *= 2;

  map->buckets = (Pair **) malloc(sizeof(Pair*) * map->capacity); 
  if (map->buckets == NULL)
  {
    exit(EXIT_FAILURE);
  }

  map->size = 0;
  for (int i = 0; i < map->capacity; i++)
    {
      map->buckets[i] = NULL;
    }

  for (int i = 0; i < map->capacity/2; i++)
    {
      if (auxBuckets[i] != NULL && auxBuckets[i]->key != NULL)
      {
        insertMap(map, auxBuckets[i]->key, auxBuckets[i]->value);
      }
    }
  free(auxBuckets);
}


HashMap * createMap(long capacity) // Función 1.
{
  HashMap * map = (HashMap *) malloc(sizeof(HashMap));
  if (map == NULL)
  {
    exit(EXIT_FAILURE);
  }

  map->capacity = capacity;
  map->size = 0;
  map->current = -1;

  map->buckets = (Pair **) malloc(sizeof(Pair*) * capacity);
  if (map->buckets == NULL)
  {
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < capacity; i++)
    {
      map->buckets[i] = NULL;
    }
  
  return map;
}

void eraseMap(HashMap * map,  char * key) // Función 4
{    
  Pair * parAux = searchMap(map, key);

  if (parAux != NULL)
  {
    parAux->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) // Función 3.
{
  long numHash = hash(key, map->capacity);
  Pair * parAux = NULL;

  while (map->buckets[numHash] != NULL)
    {
      if (map->buckets[numHash] != NULL && strcmp(map->buckets[numHash]->key, key) == 0)
      {
        parAux = map->buckets[numHash];
        break;
      }
      numHash = (numHash + 1) % map->capacity;
    }

  map->current = numHash;
  
  return parAux;
}

Pair * firstMap(HashMap * map) // Función 5.1
{
  map->current = -1;

  for (int i = 0; i < map->capacity; i++)
    {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
    }
  
  return NULL;
}

Pair * nextMap(HashMap * map)
{
  if (map->current == -1)
  {
    return firstMap(map);
  }
  else
  {
    for (int i = map->current + 1; i < map->capacity; i++)
      {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
          map->current = i;
          return map->buckets[i];
        }
      }
  }
  return NULL;
}
