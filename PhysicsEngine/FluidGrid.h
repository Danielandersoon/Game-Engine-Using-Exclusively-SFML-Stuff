#ifndef FLUID_GRID_H
#define FLUID_GRID_H
#include <vector>
#include "./FluidCell.h"

namespace GUESS::physics {
    class FluidGrid {
    private:
        std::vector<std::vector<FluidCell>> cells;
        size_t width;
        size_t height;
        float cellSize;  // Physical size of each cell in meters
        float gravity;   // m/s²

    public:
        FluidGrid(size_t w, size_t h, float resolution = 1.0f, float g = 9.81f) :
            width(w),
            height(h),
            cellSize(1.0f / resolution),
            gravity(g) {
            cells.resize(height, std::vector<FluidCell>(width, FluidCell(cellSize)));
        }

        void updateCells();
        void updateThermodynamics();
        void handleStateTransitions();
        void calculatePressures();
        void handleFluidMixing();

        FluidCell& getCell(size_t x, size_t y) { return cells[y][x]; }
        float getCellSize() const { return cellSize; }
    };
}
#endif
