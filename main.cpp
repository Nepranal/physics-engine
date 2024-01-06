#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;
using namespace sf;

int x_screen_dim = 1280;
int y_screen_dim = 720;
float t = 0.4;


class Particle{
    private:
        RectangleShape *shape;
        Vector2f *p;
        Vector2f *original_p;
        Vector2f *next_p;
        Vector2f *a;
        Vector2f *v;

    public:
        Particle(){
            original_p = new Vector2f(600,20);
            next_p= new Vector2f(original_p->x,original_p->y);
            shape = new RectangleShape(Vector2f(100,100));
            v = new Vector2f(0,0);
            p= new Vector2f(original_p->x,original_p->y);
            a = new Vector2f(0,9.81);
            shape->setPosition(*p);
        }

        RectangleShape* getShape(){
            return shape;
        }

        Vector2f* getPosition(){
            return p;
        }

        Vector2f* getNextPosition(){
            return next_p;
        }

        Vector2f* getOriginalPosition(){
            return original_p;
        }

        Vector2f* getVelocity(){
            return v;
        }

        Vector2f* getAcceleration(){
            return a;
        }

};



void resolveCollision(float *next_p, float *original_p, float *v, float upper_bound){
    if(*next_p>upper_bound-100 || *next_p<0){
        *v=-(*v);

        *original_p = upper_bound-100;
        if(*next_p<0){
            *original_p=0;
        }
    }
}

float nextVelocity(float *v, float *a, float t){
    return *v + (*a)*t;
}

//Next position logic
void update(RectangleShape *rec, Vector2f *p, Vector2f *next_p, Vector2f *original_p, Vector2f *v, Vector2f *a){
    next_p->x = original_p->x + v->x * t + 0.5 * a->x * pow(t,2);
    next_p->y = original_p->y+v->y*t + 0.5 * a->y * pow(t,2);

    v->x = nextVelocity(&(v->x), &(a->x), t);
    v->y = nextVelocity(&(v->y), &(a->y), t);

    resolveCollision(&(next_p->x), &(original_p->x), &(v->x), x_screen_dim);
    resolveCollision(&(next_p->y), &(original_p->y), &(v->y), y_screen_dim);

    original_p->y = next_p->y;
    original_p->x = next_p->x;
    p->x = next_p->x;
    p->y = next_p->y;

    rec->setPosition(*p);
}

//Wrapper function
void update(Particle *p){
    update(p->getShape(), p->getPosition(), p->getNextPosition(), p->getOriginalPosition(), p->getVelocity(), p->getAcceleration());
}

int main(){
    RenderWindow window(VideoMode(x_screen_dim,y_screen_dim),"window");
    window.setFramerateLimit(60);

    //Object initialization
    Particle *p = new Particle();
    
    while(window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type==sf::Event::Closed) window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        window.clear();
        window.draw(*(p->getShape()));
        window.display();

        // update(rec,p,next_p,original_p,v,a);
        update(p);
    }

    

    return 0;
}