#ifndef SLHTML_H
#define SLHTML_H

#include <stdlib.h>
#include <stdio.h>

typedef struct slhtml_s {
    char     *tags[64];
    size_t    tagsz;
    int     (*cmd) (void *_udata, char *_tag, char *_body);
    int     (*snd) (void *_udata, char *_data, size_t _datasz);
    void     *udata;
} slhtml_t;

extern size_t slhtml_findtag(char *, char *[], size_t, char **);
extern int    slhtml_process(slhtml_t *, char *);

#endif
