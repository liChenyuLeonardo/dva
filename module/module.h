#ifndef _MODULE_H_INCLUDED_
#define _MODULE_H_INCLUDED_
#include <string>
#include <functional>
#include <unordered_map>
using std::string;
/*所有的模块将在此头文件中包括*/
#include "hongniu.h"

struct module_t
{
    string name;
    std::function<void()> handler;
    template<typename F, typename... Args>
    module_t(string _name, F&& functor, Args&&... args):
    name(_name), handler(std::bind(std::forward<F>(functor), std::forward<Args>(args)...)){}
    module_t(const module_t&) = default;
};

void test(int a, int b){
	//cout << "test succeeded!" << endl;
    return;
}
module_t moduleList[] = {
    module_t("test",test, 1, 2) 
};

class module
{
private:
    std::unordered_map<string, module_t*> modules;   
    void addModule(module_t& theModule);
public:
    module(){
        for(module_t& a : moduleList)
            addModule(a);
    };
    module(const module&) = delete;
    module& operator= (const module&) = delete;
    module_t* getModuleByName(const string& moduleName);
    ~module() = default;
};

void module::addModule(module_t& theModule)
{
    modules.emplace(theModule.name, &theModule);
}

module_t* module::getModuleByName(const string& moduleName)
{
    using it_t = std::unordered_map<string, module_t*>::iterator;
    it_t theModule = modules.find(moduleName);
    return theModule->second;
     
}
#endif