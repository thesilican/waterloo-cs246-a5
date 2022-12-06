from PIL import Image
import os.path

files = [
    ("BOARD", "board.png"),
    ("BLACK_KING", "black-king.png"),
    ("WHITE_KING", "white-king.png"),
    ("BLACK_BISHOP", "black-bishop.png"),
    ("WHITE_BISHOP", "white-bishop.png"),
    ("BLACK_KNIGHT", "black-knight.png"),
    ("WHITE_KNIGHT", "white-knight.png"),
    ("BLACK_PAWN", "black-pawn.png"),
    ("WHITE_PAWN", "white-pawn.png"),
    ("BLACK_QUEEN", "black-queen.png"),
    ("WHITE_QUEEN", "white-queen.png"),
    ("BLACK_ROOK", "black-rook.png"),
    ("WHITE_ROOK", "white-rook.png"),
    ("ORANGE_SQUARE", "orange-square.png"),
    ("YELLOW_SQUARE", "yellow-square.png"),
    ("TEXT_BLACK_CHECK", "text-black-check.png"),
    ("TEXT_BLACK_CHECKMATE", "text-black-checkmate.png"),
    ("TEXT_BLACK_WINS", "text-black-wins.png"),
    ("TEXT_WHITE_CHECK", "text-white-check.png"),
    ("TEXT_WHITE_CHECKMATE", "text-white-checkmate.png"),
    ("TEXT_WHITE_WINS", "text-white-wins.png"),
    ("TEXT_STALEMATE", "text-stalemate.png"),
]

src = '#include "assets.h"\n\n/* File generated by assets/gen.py */\n\n'
header = '#ifndef ASSETS_H\n#define ASSETS_H\n#include "window.h"\n\n/* File generated by assets/gen.py */\n\n'

for varname, filename in files:
    filepath = os.path.join(os.path.dirname(__file__), filename)
    img = Image.open(filepath).convert(mode="RGBA")
    src += f"PixelImg {varname} = PixelImg({img.width}, {img.height}, {{"
    for j in range(img.height):
        if j != 0:
            src += ","
        src += "{"
        for i in range(img.width):
            if i != 0:
                src += ","
            r, g, b, a = img.getpixel((i, j))
            if a != 0:
                src += str((r << 16) + (g << 8) + b)
            else:
                src += "-1"
        src += "}"
    src += "});\n"
    header += f"extern PixelImg {varname};\n"
header += "\n#endif\n"

with open(os.path.join(os.path.dirname(__file__), "../src/assets.cc"), "w") as f:
    f.write(src)
with open(os.path.join(os.path.dirname(__file__), "../src/assets.h"), "w") as f:
    f.write(header)
