#include "cJSON.h"
#include "fuse_i.h"

void save_fuse_state(struct fuse* fuse) {
    cJSON *root = cJSON_CreateObject();
    cJSON *nodes = cJSON_CreateArray();

    for (size_t i = 0; i < fuse->id_table.size; i++) {
        struct node *n = fuse->id_table.array[i];
        if (!n) continue;

		fprintf(stderr, "[libfuse] storing node id %d -> %d\n", n->nodeid, i);

        cJSON *jnode = cJSON_CreateObject();
        cJSON_AddNumberToObject(jnode, "nodeid", n->nodeid);
        cJSON_AddNumberToObject(jnode, "parent_id", n->parent ? n->parent->nodeid : 0);
        cJSON_AddNumberToObject(jnode, "generation", n->generation);
        cJSON_AddStringToObject(jnode, "name", n->inline_name);
        cJSON_AddNumberToObject(jnode, "nlookup", n->nlookup);
        cJSON_AddNumberToObject(jnode, "size", n->size);
        cJSON_AddNumberToObject(jnode, "mtime_sec", n->mtime.tv_sec);
        cJSON_AddNumberToObject(jnode, "mtime_nsec", n->mtime.tv_nsec);
		cJSON_AddNumberToObject(jnode, "open_count", n->open_count);
        cJSON_AddItemToArray(nodes, jnode);
    }

    cJSON_AddNumberToObject(root, "ctr", fuse->ctr);
    cJSON_AddNumberToObject(root, "generation", fuse->generation);
    cJSON_AddItemToObject(root, "nodes", nodes);

    char *json_str = cJSON_Print(root);
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("fopen (write)");
        exit(1);
    }
    fputs(json_str, fp);
    fclose(fp);
    free(json_str);
    cJSON_Delete(root);
}
void restore_fuse_state(struct fuse* f) {

}