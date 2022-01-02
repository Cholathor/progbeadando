#ifndef MEZO_H
#define MEZO_H
#include <string>

using namespace std;

class Mezo
{
    public:
        int x, y;
        int id;
        std::string szin;
        Termelo source;
        Fogyaszto sink;
        Futoszalag belt;

        Mezo()
        {
            id = 0;
        }
        virtual ~Mezo();

        Mezo(int xx, int yy, string sz, int id_)
        {
            x = xx;
            y = yy;
            szin = sz;
            id = id_;
        }
    protected:

    private:
};

#endif // MEZO_H
