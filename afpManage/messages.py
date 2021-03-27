import json


class Messages:

    def __init__(self):
        self.go_y_position = None
        self.go_z_position = None
        self.go_z_rotation = None
        self.go_alpha_rotation = None
        self.pGain = None
        self.dGain = None
        self.iGain = None
        self.destination = None

    def set_gain(self, pg: float, dg: float, ig: float):
        self.pGain = pg
        self.dGain = dg
        self.iGain = ig

    def set_position(self, position: int):
        self.goPosition = position

    def pack_json(self) -> str:

        return json.dumps(self.__dict__)


# def main():
#     newMessage = Messages()
#     newMessage.set_gain(0.1, 0.2, 0.3)
#     newMessage.set_position(2000)
#
#     print(newMessage.pack_json())
#
#
# main()

