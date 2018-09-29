//
// Created by Khyber on 9/29/2018.
//

#ifndef RECTANGLES_H
#define RECTANGLES_H

#include <vector>
#include <algorithm>
#include "SimulatedAnnealing.h"

struct Rectangle {
    
    double x;
    double y;
    double width;
    double height;
    
    double area() const {
        return width * height;
    }
    
    double x2() const {
        return x + width;
    }
    
    double y2() const {
        return y + height;
    }
    
};

class Rectangles {
    
    typedef std::vector<const Rectangle> RectangleVector;

private:
    
    const Rectangle bounds;
    const size_t numRectangles;
    const RectangleVector& originalRectangles;
    RectangleVector randomRectangles;
    RectangleVector rectangles;
    RectangleVector prevRectangles;
    
    double emptyArea(const Rectangle& bounds) const {
        double emptyArea = bounds.area();
        for (const Rectangle& rectangle : rectangles) {
            emptyArea -= rectangle.area();
        }
        return emptyArea;
    }
    
    /**
     * Finds the smallest bounding rectangle of everything in rectangles
     * @return smallest bounding rectangle
     */
    const Rectangle& minBounds() const {
        double x = 0;
        double y = 0;
        double x2 = 0;
        double y2 = 0;
        for (const Rectangle& rectangle : rectangles) {
            if (rectangle.x > x) {
                x = rectangle.x;
            }
            if (rectangle.y > y) {
                y = rectangle.y;
            }
            if (rectangle.x2() > x2) {
                x2 = rectangle.x2();
            }
            if (rectangle.y2() > y2) {
                y2 = rectangle.y2();
            }
        }
        return Rectangle {.x = x, .y = y, .width = x2 - x, .height = y2 - y};
    }
    
    template <typename Generator>
    void shuffle(Generator& generator) {
        randomRectangles = originalRectangles;
        std::shuffle(randomRectangles.begin(), randomRectangles.end(), generator);
    }
    
    /**
     * Attempts to layout all of Rectangles in randomRectangles into rectangles.
     */
    void doLayout() {
    
    }

public:
    
    Rectangles(const Rectangle bounds, const RectangleVector& rectangles)
            : bounds(bounds), numRectangles(rectangles.size()), originalRectangles(rectangles) {
        randomRectangles.reserve(numRectangles);
        this->rectangles.reserve(numRectangles);
        prevRectangles.reserve(numRectangles);
    }
    
    const RectangleVector& original() {
        return originalRectangles;
    }
    
    const RectangleVector& layout() {
        return rectangles;
    }
    
    /**
     * If not all rectangles fit in bounds, return empty area left.
     * If all rectangles do fit, construct a smaller bounds.
     *
     * @return empty area
     */
    double energy() const {
        return emptyArea(rectangles.size() < numRectangles ? bounds : minBounds());
    }
    
    template <typename Generator>
    void move(Generator& generator) {
        prevRectangles = rectangles;
        shuffle(generator);
        doLayout();
    }
    
    void undo() {
        rectangles = prevRectangles;
    }
    
    template <typename Temperature, typename Generator>
    const RectangleVector& anneal(const size_t numIterations, Temperature& temperature, Generator& generator) {
        if (numRectangles <= 1) {
            return originalRectangles;
        }
        return SA::anneal(*this, numIterations, temperature, generator).layout();
    }
    
    template <typename Temperature, typename Generator>
    static const RectangleVector& anneal(const Rectangle& bounds, const RectangleVector& rectangles,
                                         const size_t numIterations, Temperature& temperature, Generator& generator) {
        return Rectangles(bounds, rectangles).anneal(numIterations, temperature, generator);
    };
    
};

#endif // RECTANGLES_H
