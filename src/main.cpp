//
// Created by Khyber on 9/29/2018.
//

#include "SimulatedAnnealing.h"
#include "Rectangles.h"

#include <chrono>
#include <iostream>

template <typename Clock = std::chrono::system_clock>
class Timer {

private:
    typedef std::chrono::time_point<Clock> time_point;
    time_point _start;
    time_point _finish;

public:
    
    void start() {
        _start = Clock::now();
    }
    
    void finish() {
        _finish = Clock::now();
    }
    
    template <typename R>
    R count() {
        std::chrono::duration<R> elapsed = _finish - _start;
        return elapsed.count();
    }
    
};

class StateImpl {

private:
    
    std::normal_distribution<double>& random;
    
    double _x;
    double prevX = 0;

public:
    
    StateImpl(const double x, std::normal_distribution<double>&& random) : random(random), _x(x) {
    
    }
    
    double x() {
        return _x;
    }
    
    double energy() {
        return std::abs((_x - 10) * (_x + 20) * (_x - 30));
    }
    
    template <typename Generator>
    void move(Generator& generator) {
        prevX = _x;
        _x += random(generator);
    }
    
    void undo() {
        _x = prevX;
    }
    
    StateImpl& operator=(StateImpl copy) {
        _x = copy._x;
        return *this;
    }
    
};

class TemperatureImpl {

private:
    
    const double decayRate;
    size_t i = 0;
    double temperature;

public:
    
    TemperatureImpl(const double temperature, const double decayRate) : decayRate(decayRate), temperature(temperature) {}
    
    double next() {
        temperature *= decayRate;
        return temperature;
    }
    
};

int main() {
    std::random_device randomDevice;
    std::mt19937_64 generator(randomDevice());
    Timer<std::chrono::high_resolution_clock> timer;
    
    timer.start();
    StateImpl state(100, std::normal_distribution<double>(0, 1));
    TemperatureImpl temperature(1, .99);
    StateImpl minState = SA::anneal(state, 10000000, temperature, generator);
    std::cout << "x: " << minState.x() << ", y: " << minState.energy() << std::endl;
    timer.finish();
    std::cout << "Time: " << timer.count<double>() << std::endl;
    
    Rectangle bounds = {};
    std::vector<const Rectangle> rectangles;
    Rectangles::anneal(bounds, rectangles, 1000, temperature, generator);
    
    return 0;
}