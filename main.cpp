#include "Cluster.hpp"

int main(int argc, char* argv[])
{
// std::locale::global(std::locale(""));

    if (argc > 2) {
        std::cerr << "Error: invalid number of arguments" << std::endl;
        return 1;
    } else {
        try {
            // We check that the file at the specified path exists and parse it
            ft::Cluster cluster;
            if (argc == 2) {
                cluster.setConfig(ft::parser(argv[1]));
            // Parse the default config
            } else {
                cluster.setConfig(ft::parser(0));
            }
            cluster.setup();
            // cluster.push_back(ft::Server("127.0.0.1", "8080"));
            // cluster.push_back(ft::Server("127.0.0.1", "8000"));
            cluster.run();
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return (0);
}
