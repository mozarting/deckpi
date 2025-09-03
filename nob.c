#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv) {

    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    Nob_Cmd cmd = {0};
     
    nob_cmd_append(&cmd, "cc");             // compiler
    nob_cmd_append(&cmd, "-Wall");          // warnings
    nob_cmd_append(&cmd, "-Wextra");
    nob_cmd_append(&cmd, "-o");
    nob_cmd_append(&cmd, BUILD_FOLDER"deckpi");         // output
    nob_cmd_append(&cmd, SRC_FOLDER"main.c");         // input source
    if (!nob_cmd_run_sync(cmd)) return 1;   // run command
    nob_cmd_append(&cmd, SRC_FOLDER "file.c");   // input source

    return 0;
}
