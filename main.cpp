#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;
using namespace sf;


class Particle;


int x_screen_dim = 1280;
int y_screen_dim = 720;
float t = 0.4; //optimum: ~0.4
vector<Particle*> particles;


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
        
        void setPosition(Vector2f p){
            *this->p = p;
            shape->setPosition(p);
        }

        Vector2f* getNextPosition(){
            return next_p;
        }

        Vector2f* getOriginalPosition(){
            return original_p;
        }

        void setOriginalPosition(Vector2f original_p){
            *this->original_p = original_p;
        }

        Vector2f* getVelocity(){
            return v;
        }

        void setVelocity(Vector2f v){
            *this->v = v;
        }

        Vector2f* getAcceleration(){
            return a;
        }

};



void resolveCollision(float *next_p, float *original_p, float *v, float upper_bound){
    if(*next_p>upper_bound || *next_p<0){
        *v=-(*v);

        
        float temp = *next_p;
        *next_p = upper_bound;
        if(temp<0){
            *next_p=0;
        }
    }
}

float nextVelocity(float *v, float *a, float t){
    return *v + (*a)*t;
}

//Returns -1 if not possible
float getNextInc(float target, float p, float v, float a){
    float t;
    if(a!=0){
        float det = pow(v,2)+2*a*(target-p);
        if(det<0){
            return -1;
        }
        
        t = (-v+sqrt(det))/a;
        
    }else{
        if(v==0){
            return -1;
        }
        t = (target-p)/v;
    }

    if(t<0){
            return -1;
        }
    return t;
}

float getComponentInc(float upper_bound,float p, float v, float a){
    float t1 = getNextInc(upper_bound, p, v,a);
    float t2= getNextInc(0, p, v, a);
    float t;
    if(t1<0 || t2<0){
        t = abs(t1*t2);
    }
    return t;
}

float getNextInc( Vector2f p, Vector2f v, Vector2f a){


   
    float t_x = getComponentInc(x_screen_dim-100, p.x, v.x,a.x);
    // float t_x2= getNextInc(0, p.x, v.x,a.x);
    // if(t_x1<0 || t_x2<0){
    //     t_x = abs(t_x1*t_x2);
    // }


    float t_y = getComponentInc(y_screen_dim-100, p.y, v.y,a.y);
    // float t_y2= getNextInc(0, p.y, v.y,a.y);
    // if(t_y1<0 || t_y2<0){
    //     t_y = abs(t_y1*t_y2);
    // }


    float t_xy = min(t_x,t_y);


    if(t_xy<t){
        return t_xy;
    }

    return t;
}

//Next position logic
void update(RectangleShape *rec, Vector2f *p, Vector2f *next_p, Vector2f *original_p, Vector2f *v, Vector2f *a){
    next_p->x = original_p->x + v->x * t + 0.5 * a->x * pow(t,2);
    next_p->y = original_p->y + v->y * t + 0.5 * a->y * pow(t,2);

    float t_inc= getNextInc(*p,*v,*a);
    v->x = nextVelocity(&(v->x), &(a->x), t_inc);
    v->y = nextVelocity(&(v->y), &(a->y), t_inc);

    resolveCollision(&(next_p->x), &(original_p->x), &(v->x), x_screen_dim-100);
    resolveCollision(&(next_p->y), &(original_p->y), &(v->y), y_screen_dim-100);


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
    p->setPosition(Vector2f(200,120));
    p->setOriginalPosition(Vector2f(200,120));
    p->setVelocity(Vector2f(50,0));

    Particle *p1 = new Particle();
    p1->setPosition(Vector2f(x_screen_dim-100,620));
    p1->setOriginalPosition(Vector2f(x_screen_dim-100,620));
    p1->setVelocity(Vector2f(0,0));

    particles.push_back(p);
    particles.push_back(p1);
    
    while(window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type==sf::Event::Closed) window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        window.clear();
        window.draw(*(p->getShape()));
        // window.draw(*(p1->getShape()));
        window.display();

        // update(rec,p,next_p,original_p,v,a);
        update(p);
        // update(p1);
    }

    

    return 0;
}