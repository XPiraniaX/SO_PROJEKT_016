#ifndef SKIER_H
#define SKIER_H

using namespace std;

class Skier {
private:
    int id; 
    int num_children;

public:
    Skier(int id);  
    int getId() const;  
    int getChildren() const;
};

#endif
