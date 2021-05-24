#include <stdlib.h>
#include <raptorial.h>
#include <notcurses/notcurses.h>

#define DEFAULT_PACKAGE_DIR "/var/lib/apt/lists"

typedef struct packagedb {
  struct pkgcache* pcache;
  //struct dfa* automaton;
} packagedb;

static int
create_main_ui(struct notcurses* nc, const packagedb* pdb){
  (void)nc;
  (void)pdb; // FIXME
  return 0;
}

// load the package databases from disk into pdb
static int
load_packagedb(struct notcurses* nc, const char* dir, packagedb* pdb){
  int perr = 0;
  if((pdb->pcache = lex_packages_dir(dir, &perr, NULL)) == NULL){
    return -1;
  }
  return 0;
}

int main(void){
  const char* pdir = DEFAULT_PACKAGE_DIR;
  struct notcurses* nc = notcurses_init(NULL, NULL);
  if(!nc){
    return EXIT_FAILURE;
  }
  packagedb pdb = { };
  if(load_packagedb(nc, pdir, &pdb)){
    notcurses_stop(nc);
    return EXIT_FAILURE;
  }
  if(create_main_ui(nc, &pdb)){
    free_package_cache(pdb.pcache);
    notcurses_stop(nc);
    return EXIT_FAILURE;
  }
  free_package_cache(pdb.pcache);
  if(notcurses_stop(nc)){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
