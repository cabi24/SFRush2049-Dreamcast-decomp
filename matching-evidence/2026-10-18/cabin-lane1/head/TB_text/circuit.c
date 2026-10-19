/* Private DC circuit candidate; anonymous target function identities preserved. */
#include "circuit_types.h"

#pragma inline(_CircuitRandom)

static unsigned short _CircuitRandom(unsigned short max);

static int gCircuitPoints[6] = {10, 7, 5, 3, 2, 1};
int gNumCircuitTracks[4] = {4, 5, 6, 6};
static int gCircuitFog[4] = {0, 4, 7, 9};
static int gCircuitWind[4] = {0, 1, 2, 2};
static int gCircuitDiff[4] = {0, 1, 2, 3};
unsigned char gCircuitPlayer;
tCircuitTrack gCircuitTracks[24];
tCircuitCar gCircuitCars[6];
signed char gCircuitTrack;
signed char gCircuitSize;
static unsigned int gCircuitSeed;
static unsigned char gCircuitNumTrackRaces[6];
static unsigned char gTempCircuitFirstTrack[4];
static unsigned short gTempCircuitPoints[4][6];

void func_8C01C554(int circuitType)
{
    tCircuit *circuit; 
    circuit = &(*(*gPlayers[gCircuitPlayer].save)->datah)->circuit;
    memset(circuit, 0, sizeof(*circuit));
    PS_WriteFile((*gPlayers[gCircuitPlayer].save)->fileh, circuit,
                                                       sizeof(*circuit));
    circuit->type = circuitType;
    circuit->seed = *(unsigned int *)&gSeconds & 0xfffff;
    circuit->active = 1;
    circuit->totalTime = 0.0f;
    if (gOptions[30])
        circuit->death = 1;
    circuit->difficulty = gCircuitDiff[circuitType];
}


void func_8C01C5E4(void)
{
    int i; 
    int j; 
    int k; 
    int n; 
    int bit; 
    int b; 
    int sortList[6]; 
    int numDrones; 
    int numCircuitTracks; 
    tCircuit *circuit; 
    int orient; 
    static int skills[6][12] = {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1, 0, 0, 3,
                                2, 2, 2, 3, 3, 3, 3, 4, 2, 4, 3, 1, 3, 3, 4, 4, 4, 0, 0, 3, 3, 2, 5, 4,
                                4, 5, 5, 2, 2, 5, 5, 5, 5, 3, 4, 0, 5, 4, 3, 5, 5, 4, 4, 0, 4, 5, 1, 5};
    circuit = &(*(*gPlayers[gCircuitPlayer].save)->datah)->circuit;
    gCircuitSeed = circuit->seed;
    numCircuitTracks = gNumCircuitTracks[circuit->type];
    if (circuit->type == 3)
        gCircuitSize = numCircuitTracks * 4;
    else
        gCircuitSize = numCircuitTracks * 2;
    gCircuitTrack = circuit->track < gCircuitSize - 1 ? circuit->track : gCircuitSize - 1;
    for (i = 0; i < numCircuitTracks; i++)
        gCircuitNumTrackRaces[i] = 0;
    numDrones = 6 - gNumPlayers;
    for (i = 0; i < numDrones; i++) {
    again:
        gCircuitCars[i + gNumPlayers].name = _CircuitRandom(57);
        for (j = 0; j < i; j++)
            if (gCircuitCars[i + gNumPlayers].name == gCircuitCars[j + gNumPlayers].name)
                goto again;
    }
    for (i = 0; i < numDrones; i++)
        sortList[i] = i;
    for (i = 0; i < numDrones; i++) {
        do {
            j = _CircuitRandom(numDrones - 1);
        } while (j == i);
        sortList[i] ^= sortList[j];
        sortList[j] ^= sortList[i];
        sortList[i] ^= sortList[j];
    }
    for (i = 0; i < numDrones; i++) {
        for (j = 0; j < 12; j++)
            gCircuitCars[gNumPlayers + sortList[i]].skill[j] = skills[i][j];
    }
    for (i = 0; i < 12; i++) {
        do {
            j = _CircuitRandom(11);
        } while (i == j);
        for (k = 0; k < numDrones; k++) {
            gCircuitCars[sortList[k] + gNumPlayers].skill[i] ^=
                gCircuitCars[sortList[k] + gNumPlayers].skill[j];
            gCircuitCars[sortList[k] + gNumPlayers].skill[j] ^=
                gCircuitCars[sortList[k] + gNumPlayers].skill[i];
            gCircuitCars[sortList[k] + gNumPlayers].skill[i] ^=
                gCircuitCars[sortList[k] + gNumPlayers].skill[j];
        }
    }
    for (i = 0; i <= gCircuitTrack; i++) {
        if (circuit->type == 3) {
            gCircuitTracks[i].trackNum = _CircuitRandom(numCircuitTracks - 1);
            gCircuitTracks[i].backward = _CircuitRandom(1);
            gCircuitTracks[i].mirror = _CircuitRandom(1);
            if (i) {
                k = 0;
                while (
                    (gCircuitTracks[i].trackNum == gCircuitTracks[i - 1].trackNum && k < numCircuitTracks) ||
                    gCircuitNumTrackRaces[gCircuitTracks[i].trackNum] == 4) {
                    gCircuitTracks[i].trackNum++;
                    if (gCircuitTracks[i].trackNum >= numCircuitTracks)
                        gCircuitTracks[i].trackNum = 0;
                    k++;
                }
                do {
                    for (j = 0; j < i; j++)
                        if (gCircuitTracks[i].trackNum == gCircuitTracks[j].trackNum &&
                            gCircuitTracks[i].backward == gCircuitTracks[j].backward &&
                            gCircuitTracks[i].mirror == gCircuitTracks[j].mirror) {
                            orient = gCircuitTracks[i].backward | gCircuitTracks[i].mirror << 1;
                            orient++;
                            orient %= 4;
                            gCircuitTracks[i].backward = orient & 1;
                            gCircuitTracks[i].mirror = (orient >> 1) & 1;
                            break;
                        }
                } while (j != i);
            }
            gCircuitNumTrackRaces[gCircuitTracks[i].trackNum]++;
        } else {
            gCircuitTracks[i].trackNum = i % numCircuitTracks;
            gCircuitTracks[i].backward = i >= numCircuitTracks;
        }
        k = _CircuitRandom(gCircuitFog[circuit->type]);
        gCircuitTracks[i].fog = k < 4 ? 0 : k < 7 ? 1 : k < 9 ? 2 : 3;
        gCircuitTracks[i].wind = _CircuitRandom(gCircuitWind[circuit->type]);
    }
    n = 6 - (gNumPlayers == 1 ? 0 : 2);
    for (i = 0; i < n; i++) {
        gCircuitCars[i].totalPoints =
            circuit->temp ? gTempCircuitPoints[gPlayers[gCircuitPlayer].controller][i] : 0;
        for (j = 0; j < circuit->track; j++) {
            if (circuit->temp && j < gTempCircuitFirstTrack[gPlayers[gCircuitPlayer].controller]) {
                gCircuitCars[i].places[j] = 6;
                gCircuitCars[i].points[j] = 0;
            } else {
                gCircuitCars[i].places[j] = 0;
                bit = j * 3;
                for (b = 0; b < 3; b++, bit++)
                    gCircuitCars[i].places[j] |= ((circuit->ranks[i][bit >> 3] >> (bit & 7)) & 1) << b;
                gCircuitCars[i].points[j] = gCircuitPoints[gCircuitCars[i].places[j]];
                gCircuitCars[i].totalPoints += gCircuitCars[i].points[j];
            }
        }
        for (; j < gCircuitSize; j++) {
            gCircuitCars[i].places[j] = 6;
            gCircuitCars[i].points[j] = 0;
        }
    }
    for (; i < 6; i++) {
        gCircuitCars[i].totalPoints = 0;
        for (j = 0; j < gCircuitSize; j++) {
            gCircuitCars[i].places[j] = 6;
            gCircuitCars[i].points[j] = 0;
        }
    }
    for (i = 0; i < 6; i++)
        sortList[i] = i;
    for (i = 0; i < 5; i++)
        for (j = i + 1; j < 6; j++)
            if (gCircuitCars[sortList[i]].totalPoints < gCircuitCars[sortList[j]].totalPoints) {
                sortList[i] ^= sortList[j];
                sortList[j] ^= sortList[i];
                sortList[i] ^= sortList[j];
            }
    for (i = 0; i < 6; i++)
        gCircuitCars[sortList[i]].totalPlace = i;
}


void UpdateCircuit(unsigned char *ranks, float time)
{
    int i; 
    int n; 
    int bit; 
    int b; 
    int place; 
    int spread; 
    int diff; 
    int first; 
    int second; 
    tCircuit *circuit; 
    circuit = &(*(*gPlayers[gCircuitPlayer].save)->datah)->circuit;
    n = 6 - (gNumPlayers == 1 ? 0 : 2);
    if (gCircuitPlayer > 0) {
        ranks[0] ^= ranks[1];
        ranks[1] ^= ranks[0];
        ranks[0] ^= ranks[1];
    }
    for (i = 0; i < n; i++, ranks++) {
        bit = gCircuitTrack * 3;
        for (b = 0; b < 3; b++, bit++) {
            circuit->ranks[i][bit >> 3] =
                (circuit->ranks[i][bit >> 3] & ~(1 << (bit & 7))) | ((*ranks & 1) << (bit & 7));
            *ranks >>= 1;
        }
    }
    circuit->track++;
    circuit->totalTime += time;
    func_8C01C5E4();
    gCircuitTrack--;
    if (circuit->track >= gCircuitSize)
        RecordCircuit();
    if (circuit->type == 3) {
        if (gNumPlayers == 1)
            place = gCircuitCars[0].totalPlace;
        else
            place = (gCircuitCars[0].totalPlace < gCircuitCars[1].totalPlace ? gCircuitCars[0].totalPlace
                                                                             : gCircuitCars[1].totalPlace);
        for (i = 0; i < 6; i++) {
            if (gCircuitCars[i].totalPlace == 0)
                first = i;
            else if (gCircuitCars[i].totalPlace == 1)
                second = i;
        }
        diff = circuit->difficulty;
        spread = gCircuitCars[first].totalPoints - gCircuitCars[second].totalPoints;
        if ((place == 0 && diff < 5 && spread >= 20) || (place == 0 && diff < 4 && spread >= 6) ||
            (place < 2 && diff < 3) || (place < 3 && diff < 2) || (place < 4 && diff < 1))
            circuit->difficulty = diff + 1;
    }
    SavePlayerCircuit(gPlayers[gCircuitPlayer].save);
}


void GetCircuitCode(unsigned char *code)
{
    int i; 
    tCircuit *circuit; 
    circuit = &(*(*gPlayers[gCircuitPlayer].save)->datah)->circuit;
    func_8C01C5E4();
    gCircuitTrack--;
    NewPassword(86, 14);
    StorePasswordElement(circuit->type, 3);
    StorePasswordElement(circuit->death, 1);
    StorePasswordElement(circuit->track, 5);
    StorePasswordElement(circuit->seed, 20);
    StorePasswordElement(circuit->difficulty, 3);
    for (i = 0; i < 6; i++)
        StorePasswordElement(gCircuitCars[i].totalPoints, 9);
    GetPasswordString((char *)code);
}


int SetCircuitCode(unsigned char *code)
{
    unsigned int type; 
    unsigned int death; 
    unsigned int track; 
    unsigned int seed; 
    unsigned int diff; 
    unsigned int points[6]; 
    int i; 
    int j; 
    int sortList[6]; 
    int givenPoints; 
    int totalPoints; 
    tCircuit *circuit; 
    circuit = &(*(*gPlayers[gCircuitPlayer].save)->datah)->circuit;
    NewPassword(86, 14);
    if (!SetPasswordString((char *)code))
        return 0;
    GetPasswordElement(&type, 3);
    GetPasswordElement(&death, 1);
    GetPasswordElement(&track, 5);
    GetPasswordElement(&seed, 20);
    GetPasswordElement(&diff, 3);
    for (i = 0; i < 6; i++)
        GetPasswordElement(&points[i], 9);
    if (track == 0 || track >= gNumCircuitTracks[type] * 4)
        return 0;
    for (i = 0; i < 6; i++)
        sortList[i] = i;
    for (i = 0; i < 5; i++)
        for (j = i + 1; j < 6; j++)
            if (points[sortList[i]] < points[sortList[j]]) {
                sortList[i] ^= sortList[j];
                sortList[j] ^= sortList[i];
                sortList[i] ^= sortList[j];
            }
    if (points[sortList[0]] > track * gCircuitPoints[0])
        return 0;
    givenPoints = 0;
    for (i = 0; i < 6; i++)
        givenPoints += gCircuitPoints[i];
    totalPoints = 0;
    for (i = 0; i < 6; i++)
        totalPoints += points[sortList[i]];
    if (totalPoints != givenPoints * track)
        return 0;
    func_8C01C554(type);
    circuit->temp = 1;
    circuit->death = death;
    circuit->track = track;
    gTempCircuitFirstTrack[gPlayers[gCircuitPlayer].controller] = track;
    circuit->seed = seed;
    circuit->difficulty = diff;
    for (i = 0; i < 6; i++)
        gTempCircuitPoints[gPlayers[gCircuitPlayer].controller][i] = points[i];
    func_8C01C5E4();
    return 1;
}


static unsigned short _CircuitRandom(unsigned short max)
{
    return rand_r(&gCircuitSeed) % (max + 1u);
}
