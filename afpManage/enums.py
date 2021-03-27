import enum


class Board(enum.Enum):

    SEA = 1
    general = 2


class State(enum.Enum):
    inSetup = 0
    stopped = 1
    start = 2
    axisInit = 3
    axisInitComplete = 4
    goToStart = 5
    idle = 6
    down = 7
    up = 8
    manUp = 9
    manDown = 10
    pleaseGoToPos = 11
    posTwo = 12
