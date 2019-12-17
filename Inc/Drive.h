#ifndef DRIVE_H
#define DRIVE_H

#include "Effect.h"
#include "main.h"

class Drive : public Effect {
   private:
    uint32_t drive;
    float32_t volume;

   public:
    Drive(const char *name);
    ~Drive();
    void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size);
    void UpdateParameters();
};

#endif