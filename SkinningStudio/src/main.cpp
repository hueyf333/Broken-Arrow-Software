#include "Application.h"
#include <cstdio>

int main(int argc, char* argv[]) {
    Application app;
    
    if (!app.Initialize()) {
        fprintf(stderr, "Failed to initialize application\n");
        return 1;
    }
    
    app.Run();
    app.Shutdown();
    
    return 0;
}
