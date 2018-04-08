#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct Parameters {
    long double speedOfLight;
    long double acceleration;
    long double time; 
} Parameters;

Parameters* newParameters(long double acceleration, long double time) {
    Parameters* parameters = malloc(sizeof(Parameters));
    parameters->speedOfLight = 299792458;
    parameters->acceleration = acceleration;
    parameters->time = time;
    return parameters;
}

void destroyParameters(Parameters* parameters) {
    free(parameters);
}

void usage() {
    fprintf(stderr, "%s\n", "Usage: ConstantAcceleration acceleration_in_s_per_s_square, time{s,m,h,d}");
}

Parameters* parseParametrs(char** argv) {
    long double acceleration = strtold(argv[1], NULL);

    char unit = argv[2][strlen(argv[2]) - 1];
    // printf("%c\n", unit);

    int unitMultiplier;
    switch(unit) {
        case 's':
            unitMultiplier = 1;
            break;

        case 'm':
            unitMultiplier = 60;
            break;

        case 'h':
            unitMultiplier = 60 * 60;
            break;

        case 'd':
            unitMultiplier = 60 * 60 * 24;
            break;

        default:
            return NULL;
    }

    char* buffer = malloc(strlen(argv[2]) + 1);
    strcpy(buffer, argv[2]);
    buffer[strlen(argv[2]) - 1] = '\0';
    long double time = unitMultiplier * strtold(buffer, NULL);

    free(buffer);

    // printf("acceleration: %Lf, time: %Lf\n", acceleration, time);
    return newParameters(acceleration, time);
}

long double calculateDistanceInKilometers(Parameters* parameters) {
    return ((powl(parameters->speedOfLight, 2.0) / parameters->acceleration)
        * (coshl((parameters->acceleration * parameters->time) / parameters->speedOfLight) - 1) ) / 1000;
}

long double calculateRelativisticTime(Parameters* parameters) {
    return (parameters->speedOfLight / parameters->acceleration) * sinhl((parameters->acceleration * parameters->time) / parameters->speedOfLight);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage();
        return 1;
    }

    Parameters* parameters = parseParametrs(argv);
    if (!parameters) {
        usage();
        return 2;
    }

    printf("Distance: %Lfkm\n", calculateDistanceInKilometers(parameters));
    long double timeOnEarth = calculateRelativisticTime(parameters);
    printf("Time on earthT: %Lfs, %Lfd\n", timeOnEarth, timeOnEarth / (60 * 60 * 24));

    destroyParameters(parameters);
    return 0;
}