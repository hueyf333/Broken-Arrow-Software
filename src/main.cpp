#include "Application.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
    using namespace SkinningStudio;
    
    Application app;
    
    if (!app.Initialize()) {
        return EXIT_FAILURE;
    }
    
    app.Run();
    app.Shutdown();
    
    return EXIT_SUCCESS;
}
