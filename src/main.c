#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

typedef void (*plugin_func_t)();

int main() {
  const char *plugin_path = "../build/src/plugin/libplugin.so";
  struct stat plugin_stat;
  time_t last_mod_time = 0;

  void *plugin_handle = NULL;
  plugin_func_t plugin_function = NULL;

  while (1) {
    // Check for modification of the plugin file
    if (stat(plugin_path, &plugin_stat) == 0) {
      if (plugin_stat.st_mtime != last_mod_time) {
        // Unload the previous plugin if loaded
        if (plugin_handle) {
          dlclose(plugin_handle);
        }

        // Load the shared library
        plugin_handle = dlopen(plugin_path, RTLD_NOW);
        if (!plugin_handle) {
          fprintf(stderr, "Error loading plugin: %s\n", dlerror());
          exit(EXIT_FAILURE);
        }

        // Get the function address
        plugin_function =
            (plugin_func_t)dlsym(plugin_handle, "plugin_function");
        if (!plugin_function) {
          fprintf(stderr, "Error finding symbol: %s\n", dlerror());
          exit(EXIT_FAILURE);
        }

        last_mod_time = plugin_stat.st_mtime;
        printf("Plugin reloaded at %s", ctime(&last_mod_time));
      }
    }

    // Call the plugin function
    if (plugin_function) {
      plugin_function();
    }

    sleep(1); // Wait for a while before checking again
  }

  // Clean up
  if (plugin_handle) {
    dlclose(plugin_handle);
  }

  return 0;
}
