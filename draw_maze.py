#!usr/bin/env/python3
# -*- coding: utf-8 -*-
import numpy
import sys

# Build the vector with numpy, populate it with values, then concatenate
def read_input(filename):
    data = list()
    current = list()
    with open(filename) as img:
        for line in img.readlines():
            for value in line.rstrip('\n'):
                current.append(value)
            data.append(current)
            current = list()
    return data


def read_shape(filename):
    data = list()
    with open(filename) as img:
        for line in img.readlines():
            for value in line.rstrip('\n').split():
                data.append(value)
    data = data[4:] # Remove the ppm header
    return [data[i:i + 16 * 3] for i in range(0, len(data), 16  * 3)]


def check(data):
    size = len(data[0])
    for line in data:
        if len(line) != size:
            print("Please, make sure every line in the input file has the same number of columns.")
            raise SystemExit
    return


def load_tiles(): # Reads the shapes
    tiles = dict()
    for i in range(6):
        tiles[i] = read_shape('tiles/{}.ppm'.format(i))
    return tiles


def build(data, tiles): # Build the scene
    image = []
    current = []
    for line in data:
        for obj in line:
            current.append(numpy.vstack(tiles[int(obj)]))
        image.append(numpy.hstack(current))
        current = list()
    return image


def save_image(image, data):
    width = len(data[0]) * 16# Number of columns in each row
    height = len(data) * 16 # Number of rows
    with open('perfect_maze.ppm', 'w') as output:
        output.write('P3\n{} {}\n{}\n'.format(width, height, 255))
        for _ in image:
            for line in _:
                for obj in line:
                    output.write(str(obj) + ' ')


def main(file):
    tiles = load_tiles()
    data = read_input(file)
    check(data)
    image = build(data, tiles)
    save_image(image, data)

main(sys.argv[1])

