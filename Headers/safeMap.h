
class safeMap{

private:
    
    std::map<std::string, std::unique_ptr<std::mutex> > mutexMap;
    std::mutex insertMutex;
    std::mutex fileAddMutex;


public:

    std::map<std::string, std::vector<std::string>> mainMap;
    void insert(std::string key, std::vector<std::string> item);
    int findVector(std::string key, std::string file);

};




void safeMap::insert(std::string key, std::vector<std::string> item){


    {

        std::lock_guard<std::mutex> g_lk(insertMutex);
        mainMap.insert({key, item});
        mutexMap.insert({key, std::make_unique<std::mutex>()});

    }

}



int safeMap::findVector(std::string key, std::string file){

    std::mutex* inner_mutex;
    int flag = 0;

    std::map<std::string, std::vector<std::string>>::iterator it;


    {
        std::lock_guard<std::mutex> g_lk(fileAddMutex);

        auto it = mutexMap.find(key);
        if (it == mutexMap.end()){
            it = mutexMap.emplace(key, std::make_unique<std::mutex>()).first;
        }

        inner_mutex = it->second.get();
    }

    {
        std::lock_guard<std::mutex> c_lk(*inner_mutex);
        (mainMap[key]).push_back(file);
    }

    
    return flag;

}

