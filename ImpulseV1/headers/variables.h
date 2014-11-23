#ifndef VARIABLES_H
#define VARIABLES_H

// configuration constants
const int exponentialScalingValue = 5;
int lineSensorThreshold = (2220 - 150) / 2;
const bool exponentialControlEnabled = true;

// preset values
// structure: min, max
const int rPotValues[] = {645, 2322};
const int lPotValues[] = {502, 2352};

#endif VARIABLES_H