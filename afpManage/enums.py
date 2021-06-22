import enum
from enum import IntEnum


class Board:

    SEA = 1
    general = 2


class State(IntEnum):
    inSetup = 0
    stopped = 1
    start = 2
    axisInit = 3
    axisInitComplete = 4
    goToStart = 5
    idle = 6
    manUp = 7
    manDown = 8
    GoToPos = 9
    posTwo = 10


class StateType(IntEnum):
    SEA = 0
    multiDof = 1
    zHorizontal = 2
    zRotate = 3
    alphaRotate = 4
