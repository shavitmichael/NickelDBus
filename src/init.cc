#ifndef _GNU_SOURCE
#define _GNU_SOURCE // program_invocation_short_name
#endif
#include <dlfcn.h>
#include <errno.h> // program_invocation_short_name
#include <link.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../NickelMenu/src/failsafe.h"
// Because the failsafe uses it, we may as well too
#include "nickel_dbus.h"
#include "util.h"

#ifndef NDB_CONFIG_DIR
    #define NDB_CONFIG_DIR "/mnt/onboard/.adds/ndb"
#endif

NickelDBus *ndb;

// This is a quick and dirty alternative to goto style cleanup, which can't be used because C++
#define NDB_INIT_STOP NDB_LOG("init: done"); return
#define NDB_INIT_FS_STOP NDB_LOG("init: destroying failsafe"); nm_failsafe_destroy(fs, 5); NDB_LOG("init: done"); return

__attribute__((constructor)) void ndb_init() {
    char *err;
    NDB_LOG("version: " NDB_VERSION);
    NDB_LOG("config dir: %s", NDB_CONFIG_DIR);

    NDB_LOG("init: creating failsafe");
    nm_failsafe_t *fs;
    if (!(fs = nm_failsafe_create(&err)) && err) {
        NDB_LOG("error: could not create failsafe: %s, stopping", err);
        free(err);
        NDB_INIT_STOP;
    }

    NDB_LOG("init: checking for uninstall flag");
    if (!access(NDB_CONFIG_DIR "/uninstall", F_OK)) {
        NDB_LOG("init: flag found, uninstalling");
        nm_failsafe_uninstall(fs);
        unlink(NDB_CONFIG_DIR "/uninstall");
        NDB_INIT_STOP;
    }
    ndb = new NickelDBus(nullptr);
    if (!ndb->initSucceeded) {
        NDB_LOG("init: failed to init NickelDBus object");
        delete ndb;
        NDB_INIT_FS_STOP;
    }
    // Test assertion code
    if (ndb->testAssert(false)) {
        NDB_LOG("The test assert failed");
    }
    ndb->connectSignals();

    NDB_INIT_FS_STOP;
}