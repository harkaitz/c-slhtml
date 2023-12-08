#include "slhtml.h"
#include <string.h>
#include <stdbool.h>

size_t slhtml_findtag(char *b, char *tags[], size_t tagsz, char **_found) {
    size_t i;
    size_t j;
    for (i=0; b[i]!='\0'; i++) {
        for (j=0; j<tagsz; j++) {
            if (b[i]==tags[j][0]) {
                if (strncmp(b+i, tags[j], strlen(tags[j]))==0) {
                    if (_found) *_found = tags[j]+1;
                    return i;
                }
            }
        }
    }
    return i;
}

int    slhtml_process(slhtml_t *_s, char *_istr) {
    char          *ptr = _istr;
    size_t         shift;
    char          *found = NULL;
    
    /* Search a tag. */
    shift = slhtml_findtag(ptr, _s->tags, _s->tagsz, &found);
    _s->snd(_s->udata, ptr, shift);
    if (ptr[shift]=='\0') { return 0; }
    
    /* Advance to tag. */
    ptr += shift + 1;
    char *tag = ptr;
    
    /* Advance to tag end. */
    char *tag_ends[] = {">", "/>"};
    shift = slhtml_findtag(ptr, tag_ends, 2, NULL);
    if (ptr[shift]=='\0') {
        return 2;
    }
    ptr += shift;
    bool has_data;
    if (*ptr == '>') {
        has_data = true;
        *ptr = '\0';
        ptr++;
    } else {
        has_data = false;
        *ptr = '\0';
        ptr+=2;
    }
    
    
    /* Get data. */
    char *data;
    if (has_data) {
        data = ptr;
        size_t level = 1;
        char   tag_data_1[100];
        char   tag_data_2[100];
        char  *tag_data[2] = { tag_data_1, tag_data_2 };
        sprintf(tag_data_1, "<%s", found);
        sprintf(tag_data_2, "</%s>", found);
        while (1) {
            shift = slhtml_findtag(ptr, tag_data, 2, NULL);
            if (ptr[shift]=='\0') {
                fprintf(stderr, "[%s]\n", found);
                return 3;
            }
            ptr += shift;
            if (ptr[1]=='/') {
                level--;
            } else {
                level++;
            }
            if (level==0) {
                *ptr = '\0';
                ptr += strlen(tag_data[1]);
                break;
            } else {
                ptr += strlen(tag_data[0]);
            }
        }
    } else {
        data = "";
    }
    
    int ret = _s->cmd(_s->udata, tag, data);

    if (ret) {
        return ret;
    }
    
    return slhtml_process(_s, ptr);
}
