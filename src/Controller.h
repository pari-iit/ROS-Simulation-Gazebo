#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <vector>
#include <iomanip>
#include <utility>
#include <any>
#include "Dynamics.h"

class State;
class Control{
    double _t;
    Eigen::VectorXf _u;
    

    public:        
        Control(const double& t, const Eigen::VectorXf& u):_t(t),_u(u){}

        //IMPLEMENT RULE OF FIVE HERE
        ~Control(){
            // printf("Control at %f destroyed.\n",_t);
        }
        Control(const Control& that):_t(that._t), _u(that._u)
        {}
        Control& operator=(const Control& that){
            _t = that._t;
            _u = that._u;
            return *this;
        }
        Control(Control&& that) :
            _t(std::exchange(that._t,0)),
            _u(std::move(that._u)){}
        Control& operator=(Control&& that) {
            _t = std::exchange(that._t,0);
            _u = std::move(that._u); 
            return *this;
        }

        double time() const {return _t;}
        Eigen::VectorXf control() const {return _u;}
        
};


//Abstract class. Can define any kinds of controllers inside.
class Controller{
public:
    virtual void generateControl(const std::vector<State>& st) = 0;
    virtual std::any getGain() const = 0;
    virtual void setGain(const std::any& K) = 0;
    virtual std::vector<Control> getControl() const = 0;
    virtual void setControl(const std::vector<Control>& u) = 0;
};


/* 1. Constant control u = Kx constant feedback control. 
can be modified to include P control u = Ke. Just modify the state input.
 */
class ConstantControl:public Controller{
    Eigen::MatrixXf _K;    
    std::vector<Control> _U_t;
    Control generateControlOneStep(const State& st);
public:
    ConstantControl(const Eigen::MatrixXf& K):_K(K),_U_t(std::vector<Control>()){};
    void generateControl(const std::vector<State>& st);

    std::any getGain() const{return _K;}

    void setGain(const std::any& K);

    std::vector<Control> getControl() const {return _U_t;}

    void setControl(const std::vector<Control>& u){_U_t = u;}
};

class ProNav:public Controller{
    double _N;
    std::vector<Control> _U_t;
public:
    ProNav(const double& N):_N(N),_U_t(std::vector<Control>()){}

    void generateControl(const std::vector<State>& st);
    std::any getGain() const {return _N;};                
    void setGain(const std::any& N);
    std::vector<Control> getControl() const{return _U_t;}
    void setControl(const std::vector<Control>& u){_U_t = u;};
};


#endif