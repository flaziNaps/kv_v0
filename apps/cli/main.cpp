// Bootstrap entry point; add the CLI when the KV API is ready.
#include <iostream>
int main(int argc,char* argv[])
{
    std::cout << "共有" << argc << " 个参数" << std::endl;
    for(int i = 1; i < argc; i++)
    {
        std::cout << "参数 " << i << ": " << argv[i] << std::endl;
    }
    return 0;
}
