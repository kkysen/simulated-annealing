//
// Created by Khyber on 9/29/2018.
//

#ifndef SIMULATED_ANNEALER_H
#define SIMULATED_ANNEALER_H

#include <random>

namespace SA {
    
    template <typename State, typename Temperature, typename RandomGenerator>
    State anneal(State& state, const size_t numIterations, Temperature& temperature, RandomGenerator& generator) {
        auto energy = state.energy();
        
        State minState = state;
        auto minEnergy = energy;
        
        std::uniform_real_distribution<decltype(energy)> random(0, 1);
        
        for (size_t i = 0; i < numIterations; i++) {
            state.move(generator);
            auto nextEnergy = state.energy();
            if (nextEnergy < minEnergy) {
                minState = state;
                minEnergy = nextEnergy;
            }
            if (nextEnergy <= energy || random(generator) < std::exp((energy - nextEnergy) / temperature.next())) {
                energy = nextEnergy;
            } else {
                state.undo();
            }
        }
        
        return minState;
    };
    
}

#endif // SIMULATED_ANNEALER_H
