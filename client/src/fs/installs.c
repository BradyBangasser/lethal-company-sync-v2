#include "files.hpp"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include "../constants.hpp"

// This will be set to 1 if the checkAndInstallSystemFiles has been run
static int systemFilesVerified = 0;

/**
 * @brief Checks if a folder exists, if it doesn't, it creates it
 * 
 * @param subpath subpath of the folder in question, it will be appended to @ref LCS_PROGRAM_FILES
 * @return 1 if error, 0 if ok
 */
static inline int _verifyFolder(const char *subpath) {
    static char path[PATH_MAX];
    sprintf(path, "%s\\%s", LCS_PROGRAM_FILES, subpath);
    
    struct stat s = {0};
    if (stat(path, &s) == -1) {
        if (MKDIR(path) != 0) {
            return 1;
        }
    }

    return 0;
}

int checkAndInstallSystemFiles() {
    if (systemFilesVerified) return 0;

    struct stat s = {0};
    char infoPath[PATH_MAX];

    sprintf(infoPath, "%s\\%s", LCS_PROGRAM_FILES, LCS_INFO_FILE);

    // Verify folder will check if the folder exists and create it otherwise, returns 0 on success 1 on error
    if (_verifyFolder("")) return -1;
    if (_verifyFolder(LCS_TMP_FOLDER)) return -1;
    if (_verifyFolder(LCS_CACHE_FOLDER)) return -1;
    if (_verifyFolder(LCS_INSTALLS_FOLDER)) return -1;

    if (stat(infoPath, &s) == -1) {
        if (mkInfo(infoPath) != 0) return -1;
    }

    systemFilesVerified = 1;

    return 0;
}

int checkInstall(const char *id, char *path) {

    // GID: {6 hex characters}
    // MID: GIDXXX:{56 hex character}
    // MPID: GIDXXX:{8 hex character

    size_t result, len;
    char gameId[LCS_GID_LENGTH + 1];
    struct stat s = {0};

    len = strlen(id);

    result = checkAndInstallSystemFiles();
    if (result != 0) return -1;

    memset(path, 0, PATH_MAX);

    if (len == LCS_GID_LENGTH) {
        strncpy(gameId, id, LCS_GID_LENGTH);
        sprintf(path, "%s\\%s\\%s\\%s", LCS_PROGRAM_FILES, LCS_INSTALLS_FOLDER, id, LCS_GAME_FILE);
        
        if (stat(path, &s) == -1) return -1;
        else return 0;
    } else if (len == LCS_GID_LENGTH + 1 + LCS_MID_LENGTH) {
        char midPath[LCS_MID_LENGTH + 1] = {0};

        strncpy(gameId, id, LCS_GID_LENGTH);
        strncpy(midPath, id + 1 + LCS_GID_LENGTH, LCS_MID_LENGTH);
        snprintf(path, PATH_MAX, "%s\\%s\\%s\\%s\\%s.mod.lsf", LCS_PROGRAM_FILES, LCS_INSTALLS_FOLDER, gameId, "mods", midPath);

        if (stat(path, &s) == -1) return -1;
        else return 0;
    } else if (len == LCS_GID_LENGTH + 1 + LCS_MPID_LENGTH) {
        char midPath[LCS_MPID_LENGTH + 1] = {0};

        strncpy(gameId, id, LCS_GID_LENGTH);
        strncpy(midPath, id + 1 + LCS_GID_LENGTH, LCS_MPID_LENGTH);
        snprintf(path, PATH_MAX, "%s\\%s\\%s\\%s\\%s.modpack.lsf", LCS_PROGRAM_FILES, LCS_INSTALLS_FOLDER, gameId, "packs", midPath);

        if (stat(path, &s) == -1) return -1;
        else return 0;
    } else return -1;

    return -1;
}


int installGameByPath(const char *path, const char *gameFileContents) {
    // TODO Make gamefile verification
    if (checkAndInstallSystemFiles()) return -1;

    struct stat s = {0};
    char fpath[PATH_MAX] = {0};
    size_t result;

    if (stat(path, &s) == 0) {
        // Attempt to update the mods
        return NOT_IMPLEMENTED_YET;
    } else {
        // Create game folder
        if (MKDIR(path) != 0) return -1;

        // .game.lcs folder
        snprintf(fpath, PATH_MAX, "%s\\%s", path, LCS_GAME_FILE);

        FILE *gameFile = fopen(fpath, "w");
        if (gameFile == NULL) {
            return -1;
        }

        result = fwrite(gameFileContents, sizeof(char), strlen(gameFileContents), gameFile);
        fclose(gameFile);

        if (result <= 0) {
            return -1;
        }

        // Make the /mods dir
        snprintf(fpath, PATH_MAX, "%s\\mods", path);
        if (MKDIR(fpath) != 0) return -1;

        // Make the /packs dir
        snprintf(fpath, PATH_MAX, "%s\\packs", path);
        if (MKDIR(fpath) != 0) return -1;

        return 0;
    }
}

