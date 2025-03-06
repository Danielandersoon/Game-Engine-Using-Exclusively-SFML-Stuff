#include "./FluidGrid.h"

namespace GUESS::physics {
    void FluidGrid::updateCells() {
        calculatePressures();

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                if (cells[y][x].getState() == liquid) {
                    // Calculate fluid movement based on pressure differences and density
                    auto& currentCell = cells[y][x];
                    float currentPressure = currentCell.getProperties().pressure;
                    float currentDensity = currentCell.getProperties().density;

                    // Check neighboring cells for pressure-driven flow
                    if (y > 0) {
                        auto& lowerCell = cells[y - 1][x];
                        if (lowerCell.getState() != solid &&
                            (currentPressure > lowerCell.getProperties().pressure ||
                                currentDensity > lowerCell.getProperties().density)) {

                            float pressureDiff = currentPressure - lowerCell.getProperties().pressure;
                            GUESS::core::math::Vector2f newVel(0.0f, -pressureDiff / currentDensity);
                            currentCell.updateVelocity(newVel);
                            currentCell.mixWith(&lowerCell);
                        }
                    }
                }
            }
        }
    }

    void FluidGrid::calculatePressures() {
        for (auto& row : cells) {
            for (auto& cell : row) {
                cell.calculatePressure(gravity);
            }
        }
    }

    void FluidGrid::handleFluidMixing() {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                if (x > 0) cells[y][x].mixWith(&cells[y][x - 1]);
                if (y > 0) cells[y][x].mixWith(&cells[y - 1][x]);
            }
        }
    }

    void FluidGrid::updateThermodynamics() {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                if (x > 0) cells[y][x].transferHeat(&cells[y][x - 1]);
                if (y > 0) cells[y][x].transferHeat(&cells[y - 1][x]);
            }
        }
    }

    void FluidGrid::handleStateTransitions() {
        for (auto& row : cells) {
            for (auto& cell : row) {
                cell.updateState();
            }
        }
    }
}
