#include <catch2/catch_session.hpp>

int main(int argc, char** argv)
{
    Catch::Session session;

    int opt1 = 0; // leaving here for future reference

    auto cli = session.cli()
        | Catch::Clara::Opt(opt1, "opt1")
              ["--option1"]      
        ("option 1 value");          

    session.cli(cli);

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) 
        return returnCode;

    return session.run();
}
