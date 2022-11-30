#!/usr/bin/env python3
from chess import Board, Move
from termcolor import colored
import os.path
import json
import subprocess
import re


def moves_equiv(uci1, uci2, bot):
    if bot:
        promo_filter = lambda x: not (x.endswith("n") or x.endswith("b") or x.endswith("r"))
        return sorted(filter(promo_filter, uci1)) == sorted(filter(promo_filter, uci2))
    else:
        return sorted(uci1) == sorted(uci2)


def fen_equiv(fen1, fen2, bot):
    """Compare two fen strings, ignoring half move and full move clocks for the bot"""
    if bot:
        m1 = re.match(r"^(.+) \d+ \d+$", fen1)
        m2 = re.match(r"^(.+) \d+ \d+$", fen2)
        return m1 and m2 and m1.group(1) == m2.group(1)
    else:
        return fen1 == fen2


def program_gen_moves(fen, bot):
    cmd = "gen-moves-bot" if bot else "gen-moves"
    output = subprocess.run(["bin/chess", "--test", cmd, fen], stdout=subprocess.PIPE)
    return output.stdout.decode("utf8").splitlines()


def program_apply_move(fen, uci, bot):
    cmd = "apply-move-bot" if bot else "apply-move"
    output = subprocess.run(
        ["bin/chess", "--test", cmd, fen, uci], stdout=subprocess.PIPE
    )
    return output.stdout.decode("utf8").strip()


def test_fen(name, fen, bot):
    board = Board(fen)

    # Test that the program generates the correct legal moves
    # given a board
    output_moves = sorted(program_gen_moves(fen, bot))
    expected_moves = sorted([x.uci() for x in board.generate_legal_moves()])
    if not moves_equiv(output_moves, expected_moves, bot):
        print(colored(f"Failed: {name}", "red"))
        print("Legal moves are different")
        print("Output:")
        print(" ".join(output_moves))
        print("Expected:")
        print(" ".join(expected_moves))
        return

    # Test that the program correctly generates the proper child board
    # after applying a move to the board
    fail = False
    for move in output_moves:
        output_board = program_apply_move(fen, move, bot)
        board.push(Move.from_uci(move))
        expected_board = board.fen()
        if not fen_equiv(output_board, expected_board, bot):
            print(colored(f"Failed: {name}", "red"))
            print(f"Incorrect board output after applying {move}")
            print("Output:")
            print(output_board)
            print("Expected:")
            print(expected_board)
            fail = True
        board.pop()
    if not fail:
        print(colored(f"Passed: {name}", "green"))


# Load test data
with open(os.path.join(os.path.dirname(__file__), "suite.json")) as f:
    test_data = json.load(f)

print(colored("# Testing OOP Implementation", "cyan", attrs=["bold"]))
for name, fen in test_data.items():
    test_fen(name, fen, False)
print("")
print(colored("# Testing Bot Implementation", "cyan", attrs=["bold"]))
for name, fen in test_data.items():
    test_fen(name, fen, True)
