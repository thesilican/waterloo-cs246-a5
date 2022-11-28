#!/usr/bin/env python
from chess import Board, Move
from subprocess import run, PIPE
import os.path
import json


def program_gen_moves(fen):
    output = run(["bin/chess", "--gen-moves", fen], stdout=PIPE)
    return list(output.stdout.decode("utf8").splitlines())


def program_apply_move(fen, uci):
    output = run(["bin/chess", "--apply-move", fen, uci], stdout=PIPE)
    return output.stdout.decode("utf8").strip()


def test_fen_moves(name, fen):
    board = Board(fen)
    ex_moves = sorted(x.uci() for x in board.generate_legal_moves())
    moves = sorted(program_gen_moves(fen))
    if moves != ex_moves:
        print(f"Failed: {name}")
        print("Expected moves different")
        print("Output:")
        print(" ".join(moves))
        print("Expected:")
        print(" ".join(ex_moves))
        return
    fail = False
    for move in moves:
        next = program_apply_move(fen, move)
        board.push(Move.from_uci(move))
        ex_next = board.fen()
        if next != ex_next:
            print(f"Failed: {name}")
            print(f"Expected board different for move {move}")
            print("Output:")
            print(next)
            print("Expected:")
            print(ex_next)
            fail = True
        board.pop()
    if not fail:
        print(f"Passed: {name}")


with open(os.path.join(os.path.dirname(__file__), "suite.json")) as f:
    test_data = json.load(f)
for name, fen in test_data.items():
    test_fen_moves(name, fen)
