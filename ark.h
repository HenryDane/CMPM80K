#ifndef _ARK_H
#define _ARK_H

class Ark {
public:
    int planks_count;
    int chickens;
    int cows;
    int sheep;
    int pigs;

    int x, y;
    bool on_map;
    bool exists;

    Ark(int x, int y);
    ~Ark();
};

#endif // _ARK_H
