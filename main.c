#define _POSIX_C_SOURCE 200809L
#include "slhtml.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>


const char HELP[] =
   "Usage: slhtml SCRIPT < HTML > HTML\n";

static int cmd(void *_ign, char *_tag , char *_body);
static int snd(void *_ign, char *_data, size_t _datasz);

#define error(FMT, ...) fprintf(stderr, "slhtml: error: " FMT "\n", ##__VA_ARGS__)

int main (int _argc, char *_argv[]) {
   
    int            opt,err;
    int            ret        = 1;
    char          *m_buf      = NULL;
    size_t         m_bufsz    = 10*1024*1024;
    FILE          *fp         = NULL;
    slhtml_t       slhtml     = { .cmd = cmd, .snd = snd };
    const char    *executable = NULL;
    size_t         datasz;
    
    /* Parse command line arguments. */
    if (_argc > 1 && (!strcmp(_argv[1], "-h") || !strcmp(_argv[1], "--help"))) {
        fputs(HELP, stderr);
        return 1;
    }
    while((opt = getopt (_argc, _argv, "")) != -1) {
        switch (opt) {
        case '?':
        default:
            return 1;
        }
    }

    /* Allocate memory. */
    m_buf = malloc(m_bufsz);
    assert(m_buf);
    
    /* Search script to use and put in "executable". */
    if (optind<_argc) {
        setenv("SLHTML_SCRIPT", _argv[optind], 1);
    }
    if (!(executable = getenv("SLHTML_SCRIPT"))) {
        error("Please specify an input script.");
        goto cleanup;
    }
    
    /* Execute "${SLHTML_SCRIPT} --list" to get a list of tags. */
    fp = popen("\"${SLHTML_SCRIPT}\" --list", "r");
    datasz = fread(m_buf, 1, m_bufsz, fp);
    assert(datasz < m_bufsz);
    err = pclose(fp); fp = NULL;
    if (err) {
        goto cleanup;
    }
    m_buf[datasz] = '\0';
    for (char *w = strtok(m_buf, " \n\r\t");
         w && slhtml.tagsz<64;
         w = strtok(NULL, " \n\r\t")) {   
        slhtml.tags[slhtml.tagsz] = malloc(strlen(w)+2);
        assert(slhtml.tags[slhtml.tagsz]);
        slhtml.tags[slhtml.tagsz][0] = '<';
        strcpy(slhtml.tags[slhtml.tagsz]+1, w);
        slhtml.tagsz++;
    }
    
    /* Read from standard input. */
    datasz = fread(m_buf, 1, m_bufsz, stdin);
    if (datasz == m_bufsz) {
        error("The maximun input is %ld kBytes\n", m_bufsz);
        goto cleanup;
    } else if (ferror(stdin)) {
        error("Failed reading the input.");
        goto cleanup;
    }
    m_buf[datasz] = '\0';
    
    /* Execute slhtml. */
    slhtml.udata = (void*)executable;
    ret = slhtml_process(&slhtml, m_buf);
    if (ret == 2 || ret == 3) {
        error("The input is not valid HTML: %i", ret);
        ret = 2;
    }
  cleanup:
    if (fp) pclose(fp);
    if (m_buf) free(m_buf);
    return ret;
}

static int cmd(void *_exe, char *_tag, char *_body) {
    char cmd[strlen(_exe)+strlen(_tag)+2];
    sprintf(cmd, "%s %s", (char*)_exe, _tag);
    fflush(stdout);
    FILE *fp = popen(cmd, "w");
    if (!fp) {
        error("%s: %s", (char*)_exe, strerror(errno));
        return 1;
    }
    fputs(_body, fp);
    fflush(fp);
    fflush(stdout);
    return pclose(fp);   
}
static int snd(void *_ign __attribute__((unused)), char *_data, size_t _datasz) {
    fwrite(_data, 1, _datasz, stdout);
    return 0;
}
