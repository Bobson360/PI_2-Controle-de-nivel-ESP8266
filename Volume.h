#ifndef Volume_h
#define Volume_h

#define pi 3.14159265359

class Volume{
    private:
        float radius;
        float height;
        float distance;

    public: Volume( float Radius, float Height ){ 
        radius = Radius;
        height = Height;
    }

    float vol( double Distance ){
       
        return pi * pow(radius, 2) * ( height - Distance - 5 ) / 1000; 
    }

    
};
#endif
