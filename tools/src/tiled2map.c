#include <stdio.h>
#include <stdlib.h>

#include "../../shared/files/level.h"
#include "../../vendor/c-simple-json-parser/json.h"

const char *read_file(const char *path) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Expected file \"%s\" not found", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(len + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Unable to allocate memory for file");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, len, file);
    buffer[len] = '\0';
    fclose(file);

    return (const char *)buffer;
}

int *parseTileset(const char *tilesetFile) {
    const char *tileset = read_file(tilesetFile);
    if (tileset == NULL) {
        fprintf(stderr, "Error reading tileset file\n");
        return NULL;
    }

    result(json_element) rootNode = json_parse(tileset);
    if (result_is_err(json_element)(&rootNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&rootNode);
        fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
        free((void *)tileset);
        return NULL;
    }

    // Extract root node data
    typed(json_element) root = result_unwrap(json_element)(&rootNode);

    // Get tilecount
    result(json_element) tileCountNode =
        json_object_find(root.value.as_object, "tilecount");
    if (result_is_err(json_element)(&tileCountNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&tileCountNode);
        fprintf(stderr, "Error getting element \"tilecount\": %s\n",
                json_error_to_string(error));
        free((void *)tileset);
        return NULL;
    }

    typed(json_element) tileCount = result_unwrap(json_element)(&tileCountNode);

    // Get tileset tiles data
    result(json_element) tilesNode =
        json_object_find(root.value.as_object, "tiles");
    if (result_is_err(json_element)(&tilesNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&tilesNode);
        fprintf(stderr, "Error getting element \"tiles\": %s\n",
                json_error_to_string(error));
        free((void *)tileset);
        return NULL;
    }

    typed(json_element) tilesArray = result_unwrap(json_element)(&tilesNode);

    int count = tileCount.value.as_number.value.as_long;
    int *tiles = (int *)malloc(sizeof(int) * count);
    if (tiles == NULL) {
        free((void *)tileset);  // Free allocated memory for `tileset`
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        typed(json_element) tileNode = tilesArray.value.as_array->elements[i];

        result(json_element) tileIdNode =
            json_object_find(tileNode.value.as_object, "id");
        if (result_is_err(json_element)(&tileIdNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&tileIdNode);
            fprintf(stderr, "Error getting element \"id\": %s\n",
                    json_error_to_string(error));
            free((void *)tileset);
            return NULL;
        }

        result(json_element) tileTypeNode =
            json_object_find(tileNode.value.as_object, "type");
        if (result_is_err(json_element)(&tileTypeNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&tileTypeNode);
            fprintf(stderr, "Error getting element \"type\": %s\n",
                    json_error_to_string(error));
            free((void *)tileset);
            return NULL;
        }

        typed(json_element) tileId = result_unwrap(json_element)(&tileIdNode);
        typed(json_element) tileType = result_unwrap(json_element)(&tileTypeNode);

        if (i == tileId.value.as_number.value.as_long) {
            tiles[i] = atoi(tileType.value.as_string);
        } else {
            tiles[i] = 0;
        }
    }

    free((void *)tileset);
    json_free(&root);

    return tiles;
}

Map *parseMap(const char *tilemapFile, int *tileTypes) {
    const char *tilemap = read_file(tilemapFile);
    if (tilemap == NULL) {
        fprintf(stderr, "Error reading tilemap file\n");
        return NULL;
    }

    result(json_element) rootNode = json_parse(tilemap);
    if (result_is_err(json_element)(&rootNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&rootNode);
        fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) root = result_unwrap(json_element)(&rootNode);

    // Get tile width & tile height
    result(json_element) tileHeightNode =
        json_object_find(root.value.as_object, "tileheight");
    if (result_is_err(json_element)(&tileHeightNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&tileHeightNode);
        fprintf(stderr, "Error getting element \"tileHeight\": %s\n",
                json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) tileHeight = result_unwrap(json_element)(&tileHeightNode);

    result(json_element) tileWidthNode =
        json_object_find(root.value.as_object, "tilewidth");
    if (result_is_err(json_element)(&tileWidthNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&tileWidthNode);
        fprintf(stderr, "Error getting element \"tileWidth\": %s\n",
                json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) width = result_unwrap(json_element)(&tileWidthNode);

    //  Get map width & height
    result(json_element) mapHeightNode =
        json_object_find(root.value.as_object, "height");
    if (result_is_err(json_element)(&mapHeightNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&mapHeightNode);
        fprintf(stderr, "Error getting element \"height\": %s\n",
                json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) mapHeight = result_unwrap(json_element)(&mapHeightNode);

    result(json_element) mapWidthNode =
        json_object_find(root.value.as_object, "width");
    if (result_is_err(json_element)(&mapWidthNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&mapWidthNode);
        fprintf(stderr, "Error getting element \"width\": %s\n",
                json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) mapWidth = result_unwrap(json_element)(&mapWidthNode);

    // Parse layers data. We only take the first layer and ignore the rest
    result(json_element) layersNode =
        json_object_find(root.value.as_object, "layers");
    if (result_is_err(json_element)(&layersNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&layersNode);
        fprintf(stderr, "Error getting element \"layers\": %s\n",
                json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) layersArray = result_unwrap(json_element)(&layersNode);
    // only pick the first layer
    typed(json_element) layer = layersArray.value.as_array->elements[0];

    result(json_element) dataNode =
        json_object_find(layer.value.as_object, "data");
    if (result_is_err(json_element)(&dataNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&dataNode);
        fprintf(stderr, "Error getting element \"data\": %s\n",
                json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) dataIds = result_unwrap(json_element)(&dataNode);
    int mapSize = (int)mapWidth.value.as_number.value.as_long *
                  (int)mapHeight.value.as_number.value.as_long;

    // Create map struct
    Map *map = malloc(sizeof(Map) + sizeof(Tile) * mapSize);
    map->width = (int)mapWidth.value.as_number.value.as_long;
    map->height = (int)mapHeight.value.as_number.value.as_long;
    map->tileWidth = (int)width.value.as_number.value.as_long;
    map->tileHeight = (int)tileHeight.value.as_number.value.as_long;

    for (int i = 0; i < mapSize; i++) {
        int tileId =
            (int)dataIds.value.as_array->elements[i].value.as_number.value.as_long;
        Tile tile = {tileTypes[tileId] << 4, tileId};
        map->data[i] = tile;
    }

    free((void *)tilemap);

    return map;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <tilemap_file> <tileset_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *tilemap = argv[1];
    const char *tileset = argv[2];
    const char *output = argv[3];

    // Prevent overwrite input
    if (tilemap == output || tileset == output) {
        perror("Error output file must be different from any input file\n");
        return EXIT_FAILURE;
    }

    // parse tileset json file and return a pointer to an array of tile types
    int *tileTypes = parseTileset(tileset);

    // parse map json file and return a pointer to an array of Tile structs
    Map *map = parseMap(tilemap, tileTypes);

    // write output map file
    FILE *outFile = fopen(output, "wb");

    // write map and tile dimensions
    fwrite(map, sizeof(uint8_t), 4, outFile);

    // write map data
    fwrite(map->data, sizeof(Tile) * map->width * map->height, 1, outFile);

    fclose(outFile);
    free((void *)map);
    free((void *)tileTypes);

    // delete output file if something fails
    if (tileTypes == NULL || map == NULL) {
        remove(output);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}