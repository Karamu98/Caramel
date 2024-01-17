﻿
extern Caramel::Application* CreateApplication();

int main(int argc, char *argv[])
{
    Caramel::Log::Init();
    CL_CORE_TRACE("Logger initialised");

    auto app = CreateApplication();
    app->Run();
    delete app;

    CL_CORE_TRACE("Client application shutdown");
    
    return 0;
}