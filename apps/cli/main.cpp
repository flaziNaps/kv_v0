// Bootstrap entry point; add the CLI when the KV API is ready.
#include <iostream>
#include <string_view>
#include <vector>
#include <span>
#include <unordered_map>
#include <sstream>
#include <kv/core/memory_store.hpp>
// 上下文结构体，用于传递命令行参数
struct Context{
    kv::Store* store;//store接口
    std::span<std::string> argv;
};

//命令结构体
struct Command{
    std::string_view name;
    void (*proc)(const Context&);
    int arity;//参数数量要求：正数表示严格相等，负数（如 -3）表示至少需要 abs(-3) 个参数
};

//命令解析函数，将输入行拆分为命令和参数
void commandParser(const std::string& line,std::vector<std::string>& argv){
    std::istringstream iss(line);
    std::string arg;
    while(iss >> arg){
        argv.push_back(arg);
    }
}

//调用背后实际的set方法
void setCommand(const Context& c){
    c.store->set(c.argv[1], c.argv[2]);
    std::cout<< "set key sucess" <<std::endl;
}

//调用get方法
void getCommand(const Context& c){
    auto value = c.store->get(c.argv[1]);
    std::cout << (value ? *value : "Key not found") << std::endl;  
}

//调用del方法
void delCommand(const Context& c){
    bool deleted = c.store->del(c.argv[1]);
    std::cout << "del key " << c.argv[1] << (deleted ? " sucess" : " failed") << std::endl;
}

//调用contains方法
void containsCommand(const Context& c){
    bool exists = c.store->contains(c.argv[1]);
    std::cout <<  (exists ? " true" : " false") << std::endl;
}

//调用showAll方法
void showAllCommand(const Context& c){
    c.store->showAll();
}


//全局静态命令表
inline constexpr Command commandTable[] = {
    {"set", setCommand,-2},
    {"get",getCommand,1},
    {"del",delCommand,1},
    {"contains",containsCommand,1},
    {"showAll",showAllCommand,0},
};


class Server{
private:
    std::unordered_map<std::string_view,const Command*> commands_;
public:
    void init(){
        for(const auto& cmd: commandTable){
            commands_[cmd.name] = &cmd;
        }
    }

    void processCommand(const Context& c){
        if(c.argv.empty()){
            std::cout << "No command provided" << std::endl;
            return;
        }
        // 查找命令是否存在哈希表中
        auto it = commands_.find(c.argv[0]);
        if(it == commands_.end()){//end用来判断是否找不到
            std::cout << "Unknown command: " << c.argv[0] << std::endl;
            return;
        }
        // 获取命令对象和参数数量
        const Command* cmd = it->second;//first指的key，second指的是value
        int argCount = c.argv.size() - 1; // Exclude the command itself
        std::cout << argCount << std::endl;
        //判断参数数量是否符合要求
        if((cmd->arity >= 0 && argCount != cmd->arity) || (cmd->arity < 0 && argCount < -cmd->arity)){
            std::cout << "Invalid number of arguments for command: " << cmd->name << std::endl;
            return;
        }
        // 调用处理函数
        cmd->proc(c);
    }
    

};

//主函数
int main()
{
    // 初始化全局命令表
    Server server;
    server.init();
    //开始读取命令行输入
    std::string line;
    kv::MemoryStore store;
    while(true){
        //显示提示符
        std::cout << "kvstore> " << std::flush;
        
        //等待用户输入一整行
        if(!std::getline(std::cin, line)){
            break;// 处理输入结束（如 Ctrl+D）
        }

        // 分割解析命令
        std::vector<std::string> argv;
        commandParser(line,argv);
        //TODO 可以添加一个判断存储方式的方法
        // 创建上下文对象，处理命令
        Context ctx{&store, argv};
        server.processCommand(ctx);
    }
    return 0;
}
