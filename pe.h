#ifndef PES_H
#define PES_H
// this is .h fle so it contains decrlarations
//
#include <stdint.h>
#include <stddef.h>

#define HASH_SIZE 32
#define HASH_HEX_SIZE (HASH_SIZE * 2)

#define OBJECTS_DIR ".pes/objects"

typedef enum {
    OBJ_BLOB,
    OBJ_TREE,
    OBJ_COMMIT
} ObjectType;
// let we define struct
typedef struct {
    uint8_t hash[HASH_SIZE];
} ObjectID;

// object functions
void hash_to_hex(const ObjectID *id, char *hex_out);
int hex_to_hash(const char *hex, ObjectID *id_out);
void compute_hash(const void *data, size_t len, ObjectID *id_out);

void object_path(const ObjectID *id, char *path_out, size_t path_size);
int object_exists(const ObjectID *id);

int object_write(ObjectType type, const void *data, size_t len, ObjectID *id_out);
int object_read(const ObjectID *id, ObjectType *type_out, void **data_out, size_t *len_out);

#endif
