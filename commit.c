#include "pes.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HEAD_FILE ".pes/HEAD"

// ─── HELPER: read HEAD ─────────────────────────
int head_read(ObjectID *out) {
    FILE *fp = fopen(HEAD_FILE, "r");
    if (!fp) return -1;

    char hex[HASH_HEX_SIZE + 1];
    if (fscanf(fp, "%64s", hex) != 1) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return hex_to_hash(hex, out);
}

// ─── HELPER: update HEAD ───────────────────────
int head_update(const ObjectID *id) {
    FILE *fp = fopen(HEAD_FILE, "w");
    if (!fp) return -1;

    char hex[HASH_HEX_SIZE + 1];
    hash_to_hex(id, hex);

    fprintf(fp, "%s\n", hex);
    fclose(fp);
    return 0;
}

// ─── CREATE COMMIT ─────────────────────────────
int commit_create(const char *message) {
    Index idx;

    if (index_load(&idx) != 0) {
        printf("error: failed to load index\n");
        return -1;
    }

    if (idx.count == 0) {
        printf("nothing to commit\n");
        return -1;
    }

    // Build simple commit content
    char content[1024];
    time_t now = time(NULL);

    snprintf(content, sizeof(content),
             "message: %s\n"
             "time: %ld\n",
             message, now);

    // store commit as object
    ObjectID commit_id;
    if (object_write(OBJ_COMMIT, content, strlen(content), &commit_id) != 0) {
        printf("error: failed to write commit\n");
        return -1;
    }

    // update HEAD
    head_update(&commit_id);

    printf("Committed: %s\n", message);
    return 0;
}

// ─── LOG HISTORY ───────────────────────────────
int commit_log() {
    ObjectID id;

    if (head_read(&id) != 0) {
        printf("No commits yet\n");
        return -1;
    }

    ObjectType type;
    void *data;
    size_t len;

    if (object_read(&id, &type, &data, &len) != 0) {
        printf("error reading commit\n");
        return -1;
    }

    printf("=== Commit ===\n");
    printf("%.*s\n", (int)len, (char *)data);

    free(data);
    return 0;
}
int commit_walk() {
    ObjectID id;

    if (head_read(&id) != 0) {
        printf("No commits yet\n");
        return -1;
    }

    ObjectType type;
    void *data;
    size_t len;

    if (object_read(&id, &type, &data, &len) != 0) {
        printf("error reading commit\n");
        return -1;
    }

    printf("=== Commit ===\n");
    printf("%.*s\n", (int)len, (char *)data);

    free(data);
    return 0;
}
