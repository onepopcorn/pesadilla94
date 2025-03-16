#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../shared/files/level.h"
#include "../../vendor/c-simple-json-parser/json.h"

#pragma pack(1)

/**
 * @brief Reads a file and returns its content as a string
 *
 * @param path The path to the file
 * @return const char* The file content
 */
char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");  // Open in binary mode
    if (!file) {
        fprintf(stderr, "Error: Cannot open file \"%s\"\n", path);
        return NULL;
    }

    // Get file size
    if (fseek(file, 0, SEEK_END) != 0) {  // Handle seek error
        fclose(file);
        return NULL;
    }

    long len = ftell(file);
    if (len == -1) {  // Handle error
        fclose(file);
        return NULL;
    }

    rewind(file);  // Move back to the beginning

    // Allocate buffer
    char *buffer = malloc(len + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read file into buffer
    size_t bytesRead = fread(buffer, 1, len, file);
    fclose(file);

    // Ensure read success
    if (bytesRead != (size_t)len) {
        fprintf(stderr, "Error: File read failed\n");
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';  // Null-terminate
    return buffer;       // Caller must free this
}

/**
 * @brief Finds the stairs data by ID
 *
 * @param stairsData The stairs data array
 * @param id The ID to find
 * @param count The number of stairs data
 * @return StairsData The stairs data
 */
StairsData findStairData(StairsData *data, int tileIdx, int count) {
    for (int i = 0; i < count; i++) {
        if (data[i].tileIdx == tileIdx) {
            return data[i];
        }
    }

    return (StairsData){-1, -1, -1};
}

int findStairIdxByStairId(StairsData *data, int id, int count) {
    for (int i = 0; i < count; i++) {
        if (data[i].id == id) {
            return data[i].tileIdx;
        }
    }

    return 0;
}

/**
 * @brief Updates the stairs data to use the tilemap tile index for the ID and the Up/Down values
 *
 * @param stairsData The stairs data array
 * @param mapWidth The map width
 * @param tileWidth The tile width
 * @param tileHeight The tile height
 * @return StairsData* The updated stairs data
 */
StairsData *transformStairsData(typed(json_element) stairsData, int mapWidth, int tileWidth, int tileHeight) {
    if (stairsData.value.as_array->count < 1) {
        return NULL;
    }

    StairsData *data = malloc(sizeof(StairsData) * stairsData.value.as_array->count);

    // Update ID to use tilemap tile index
    for (int i = 0; i < stairsData.value.as_array->count; i++) {
        typed(json_element) stair = stairsData.value.as_array->elements[i];

        // Get ID
        result(json_element) idNode = json_object_find(stair.value.as_object, "id");
        if (result_is_err(json_element)(&idNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&idNode);
            fprintf(stderr, "Error getting element \"id\": %s\n", json_error_to_string(error));
            return NULL;
        }

        typed(json_element) id = result_unwrap(json_element)(&idNode);

        // Get X value
        result(json_element) xNode = json_object_find(stair.value.as_object, "x");
        if (result_is_err(json_element)(&xNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&xNode);
            fprintf(stderr, "Error getting element \"x\": %s\n", json_error_to_string(error));
            return NULL;
        }

        typed(json_element) x = result_unwrap(json_element)(&xNode);

        // Get Y value
        result(json_element) yNode = json_object_find(stair.value.as_object, "y");
        if (result_is_err(json_element)(&yNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&yNode);
            fprintf(stderr, "Error getting element \"y\": %s\n", json_error_to_string(error));
            return NULL;
        }

        typed(json_element) y = result_unwrap(json_element)(&yNode);

        // Get coordinates
        int xCoord = (int)x.value.as_number.value.as_long / tileWidth;
        int yCoord = (int)y.value.as_number.value.as_long / tileHeight;

        // Get tile index from X and Y values
        int tileIndex = (yCoord * mapWidth) + xCoord;
        data[i].id = id.value.as_number.value.as_long;
        data[i].tileIdx = tileIndex;
    }

    // Update Up/Down values based on the tilemap tile index
    int totalStairs = stairsData.value.as_array->count;
    for (int i = 0; i < totalStairs; i++) {
        typed(json_element) stair = stairsData.value.as_array->elements[i];
        result(json_element) propertiesNode = json_object_find(stair.value.as_object, "properties");

        if (result_is_err(json_element)(&propertiesNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&propertiesNode);
            fprintf(stderr, "Error getting element \"properties\": %s\n", json_error_to_string(error));
            return NULL;
        }

        typed(json_element) properties = result_unwrap(json_element)(&propertiesNode);

        for (int j = 0; j < properties.value.as_array->count; j++) {
            typed(json_element) property = properties.value.as_array->elements[j];

            // Get custom property name node
            result(json_element) propertyNameNode = json_object_find(property.value.as_object, "name");
            if (result_is_err(json_element)(&propertyNameNode)) {
                typed(json_error) error = result_unwrap_err(json_element)(&propertyNameNode);
                fprintf(stderr, "Error getting element \"name\": %s\n", json_error_to_string(error));
                return NULL;
            }

            // Get custom property value node
            result(json_element) propertyValueNode = json_object_find(property.value.as_object, "value");
            if (result_is_err(json_element)(&propertyValueNode)) {
                typed(json_error) error = result_unwrap_err(json_element)(&propertyValueNode);
                fprintf(stderr, "Error getting element \"value\": %s\n", json_error_to_string(error));
                return NULL;
            }

            typed(json_element) propertyName = result_unwrap(json_element)(&propertyNameNode);
            typed(json_element) propertyValue = result_unwrap(json_element)(&propertyValueNode);

            // Get Up value
            if (strcmp(propertyName.value.as_string, "up") == 0) {
                int value = propertyValue.value.as_number.value.as_long;
                if (value > 0) {
                    int idx = findStairIdxByStairId(data, value, totalStairs);
                    data[i].up = idx;
                }
            }

            // Get Down value
            if (strcmp(propertyName.value.as_string, "down") == 0) {
                int value = propertyValue.value.as_number.value.as_long;
                if (value > 0) {
                    int idx = findStairIdxByStairId(data, (int)propertyValue.value.as_number.value.as_long, totalStairs);
                    data[i].down = idx;
                }
            }
        }
    }

    return data;
}

/**
 * @brief Parses a tileset JSON file and returns an array of tile types
 *
 * @param tilesetFile The path to the tileset JSON file
 * @return int* The array of tile types
 */
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
        fprintf(stderr, "Error getting element \"tilecount\": %s\n", json_error_to_string(error));
        free((void *)tileset);
        return NULL;
    }

    typed(json_element) tileCount = result_unwrap(json_element)(&tileCountNode);

    // Get tileset tiles data
    result(json_element) tilesNode = json_object_find(root.value.as_object, "tiles");
    if (result_is_err(json_element)(&tilesNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&tilesNode);
        fprintf(stderr, "Error getting element \"tiles\": %s\n", json_error_to_string(error));
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
            fprintf(stderr, "Error getting element \"id\": %s\n", json_error_to_string(error));
            free((void *)tileset);
            return NULL;
        }

        result(json_element) tileTypeNode =
            json_object_find(tileNode.value.as_object, "type");
        if (result_is_err(json_element)(&tileTypeNode)) {
            typed(json_error) error = result_unwrap_err(json_element)(&tileTypeNode);
            fprintf(stderr, "Error getting element \"type\": %s\n", json_error_to_string(error));
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

/**
 * @brief Parses a tilemap JSON file and returns a pointer to a Map struct
 * containing the tilemap data
 *
 * @param tilemapFile The path to the tilemap JSON file
 * @param tileTypes The array of tile types
 * @return Map* The parsed map data
 * @return NULL If an error occurs
 */
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
        fprintf(stderr, "Error getting element \"tileHeight\": %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) tileHeight = result_unwrap(json_element)(&tileHeightNode);

    result(json_element) tileWidthNode =
        json_object_find(root.value.as_object, "tilewidth");
    if (result_is_err(json_element)(&tileWidthNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&tileWidthNode);
        fprintf(stderr, "Error getting element \"tileWidth\": %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) width = result_unwrap(json_element)(&tileWidthNode);

    //  Get map width & height
    result(json_element) mapHeightNode =
        json_object_find(root.value.as_object, "height");
    if (result_is_err(json_element)(&mapHeightNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&mapHeightNode);
        fprintf(stderr, "Error getting element \"height\": %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) mapHeight = result_unwrap(json_element)(&mapHeightNode);

    result(json_element) mapWidthNode = json_object_find(root.value.as_object, "width");
    if (result_is_err(json_element)(&mapWidthNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&mapWidthNode);
        fprintf(stderr, "Error getting element \"width\": %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) mapWidth = result_unwrap(json_element)(&mapWidthNode);

    // Parse layers data.
    result(json_element) layersNode = json_object_find(root.value.as_object, "layers");
    if (result_is_err(json_element)(&layersNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&layersNode);
        fprintf(stderr, "Error getting element \"layers\": %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) layersArray = result_unwrap(json_element)(&layersNode);

    // First layer is the Map tile data
    typed(json_element) map_layer = layersArray.value.as_array->elements[0];

    // Second layer is the stairs data
    typed(json_element) stairs_layer = layersArray.value.as_array->elements[1];

    // Get stairs layer data. Stairs data can be empty
    result(json_element) stairsDataNode = json_object_find(stairs_layer.value.as_object, "objects");

    // Get tilemap data from map layer
    result(json_element) dataNode = json_object_find(map_layer.value.as_object, "data");
    if (result_is_err(json_element)(&dataNode)) {
        typed(json_error) error = result_unwrap_err(json_element)(&dataNode);
        fprintf(stderr, "Error getting element \"data\": %s\n", json_error_to_string(error));
        free((void *)tilemap);
        return NULL;
    }

    typed(json_element) dataIds = result_unwrap(json_element)(&dataNode);
    int mapSize = (unsigned char)mapWidth.value.as_number.value.as_long *
                  (unsigned char)mapHeight.value.as_number.value.as_long;

    // Create map struct
    Map *map = malloc(sizeof(Map) + sizeof(Tile) * mapSize);
    map->width = (unsigned char)mapWidth.value.as_number.value.as_long;
    map->height = (unsigned char)mapHeight.value.as_number.value.as_long;
    map->tileWidth = (unsigned char)width.value.as_number.value.as_long;
    map->tileHeight = (unsigned char)tileHeight.value.as_number.value.as_long;
    map->doorsCount = 0;

    // Parse stairs data
    StairsData *stairsParsedData = NULL;
    int stairsCount = 0;
    if (!result_is_err(json_element)(&stairsDataNode)) {
        typed(json_element) stairsData = result_unwrap(json_element)(&stairsDataNode);
        stairsCount = stairsData.value.as_array->count;
        stairsParsedData = transformStairsData(stairsData, map->width, map->tileWidth, map->tileHeight);
        printf("Parsing stairs data: %d stairs\n", stairsCount);
    } else {
        printf("Parsing stairs data: 0 No stairs\n");
    }

    for (int i = 0; i < mapSize; i++) {
        int tileId = (int)dataIds.value.as_array->elements[i].value.as_number.value.as_long;
        int tileType = tileTypes[tileId - 1];  // Tiled uses 1-based indexing for tileset and 0-based indexing for tilemap

        // Skip tiles that are not stairs or doors
        if (tileType != TILE_TYPE_STAIRS && tileType != TILE_TYPE_DOOR) {
            map->data[i] = (Tile){tileType, tileId, {{0, 0}}};
            continue;
        }

        // Manage stair data. Map may not contain stairs data
        if (tileType == TILE_TYPE_STAIRS && stairsParsedData != NULL && stairsCount > 0) {
            StairsData stair = findStairData(stairsParsedData, i, stairsCount);
            if (stair.id != -1) {
                map->data[i] = (Tile){tileType, tileId, {{stair.up, stair.down}}};
            }
            continue;
        }

        // Manage door data
        if (tileType == TILE_TYPE_DOOR) {
            // 254 because each frame substracts 2 and that prevents number wrapping up
            map->data[i] = (Tile){tileType, tileId, {{254}}};
            map->doorsCount++;
        }
    }

    free((void *)tilemap);
    tilemap = NULL;

    free((void *)stairsParsedData);
    stairsParsedData = NULL;

    return map;
}

/**
 * @brief Main function
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int Exit status
 */
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

    printf("\n");
    printf("Processing %s\n", tilemap);

    // parse tileset json file and return a pointer to an array of tile types
    int *tileTypes = parseTileset(tileset);
    printf("Parsing tileset: Ok\n");

    // parse map json file and return a pointer to an array of Tile structs
    Map *map = parseMap(tilemap, tileTypes);
    if (map == NULL) return EXIT_FAILURE;
    printf("Parsing map data: Ok\n\n");

    // write output map file
    FILE *outFile = fopen(output, "wb");

    // write map and tile dimensions
    fwrite(map, sizeof(char), 5, outFile);

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