import json
from enums import (Board, State, StateType)


class Messages:

    def __init__(self):
        self.go_y_position = None
        self.go_z_position = None
        self.go_z_rotation = None
        self.go_alpha_rotation = None
        self.state = None
        self.stateType = None
        # by default axis movement is set to false.
        self.move_y = False
        self.move_z = False
        self.rotate_z = False
        self.rotate_alpha = False

        self.pGain = None
        self.dGain = None
        self.iGain = None
        self.destination = None

    # setters... cause even though nothing is private, they are still useful
    def gui_set_gain(self, pg: str, dg: str, ig: str):
        self.pGain = self.input_to_float(pg)
        self.dGain = self.input_to_float(dg)
        self.iGain = self.input_to_float(ig)

    def gui_update_y_position(self, y_pos: str):
        self.go_y_position = self.input_to_int(y_pos)

        # convert the number to negative number
        if self.go_y_position > 0:
            self.go_y_position = self.go_y_position * -1

        self.move_y = False if self.go_y_position is None else True
        self.state = State.GoToPos if self.move_y else State.idle

    def gui_update_z_position(self, z_pos: str):
        self.go_z_position = self.input_to_int(z_pos)
        print(self.go_z_position)
        self.move_z = False if self.go_z_position is None else True
        print(self.move_z)
        self.state = [State.GoToPos if self.move_z else State.idle]
        self.stateType = [StateType.zHorizontal]

    def gui_update_z_rotation(self, z_pos: str):
        self.go_z_rotation = self.input_to_int(z_pos)
        self.rotate_z = False if self.go_z_rotation is None else True

    def gui_update_alpha_rotation(self, alpha_pos: str):
        self.go_alpha_rotation = self.input_to_int(alpha_pos)
        self.rotate_alpha = False if self.go_alpha_rotation is None else True

    # next two functions are for converting string inputs to numbers
    # necessary for handling bad inputs, like blank inputs
    def input_to_float(self, input: str):
        try:
            return float(input)
        except Exception as e:
            print("error: " + str(e) + " when converting " + input)
            return None

    def input_to_int(self, input: str):
        try:
            converted = int(input)
            return converted
        except Exception as e:
            print("error: " + str(e) + " when converting " + input)

            return None

    def pack_json(self) -> str:
        try:
            json_string = json.dumps(self.__dict__) + '\n'
            return json_string
        except Exception as e:
            print(e)

    def pack_byte_array(self) -> bytearray:
        try:
            json_bytes = bytearray((json.dumps(self.__dict__) + '\n'), 'utf-8')
            return json_bytes
        except Exception as e:
            print(e)

# def main():
#     newMessage = Messages()
#     newMessage.set_gain(0.1, 0.2, 0.3)
#     newMessage.set_position(2000)
#
#     print(newMessage.pack_json())
#
#
# main()

